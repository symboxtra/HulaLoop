#include "hlcontrol/internal/Record.h"

#include "hlcontrol/internal/Export.h"
#include <iostream>
#include <fstream>
#include <sndfile.h>

Record::Record(Controller *control)
{
    this->controller = control;
    this->rb = NULL;
}

void Record::start()
{
    if (this->rb == NULL)
        this->rb = this->controller->createBuffer(0.5);

    this->endRecord.store(false);
    recordThread = thread(&Record::recorder, this);
}

void Record::recorder()
{
    uint32_t samplesRead;
    float buffer[512];

    // Initialize libsndfile info.
    SF_INFO sfinfo;
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.channels = NUM_CHANNELS;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    // Create a timestamped file name
    char timestamp[20];
    time_t now = time(0);
    strftime(timestamp, 20, "%Y-%m-%d_%H-%M-%S", localtime(&now));
    std::string file_path = Export::getTempPath() + "/hulaloop_" + std::string(timestamp) + ".wav";
    SNDFILE *file = sf_open(file_path.c_str(), SFM_WRITE, &sfinfo);

    // Add file_path to vector of files
    exportPaths.push_back(file_path);

    // Add ringbuffer to buffer list
    this->controller->addBuffer(this->rb);

    // Keep recording until recording is stopped
    while (!this->endRecord.load())
    {
        samplesRead = this->rb->read(buffer, 512);

        if (samplesRead > 0)
        {
            printf("Samples read: %d\n", samplesRead);

            sf_count_t error = sf_writef_float(file, buffer, samplesRead / NUM_CHANNELS);
            if (error != samplesRead / NUM_CHANNELS)
            {
                char errstr[256];
                sf_error_str (0, errstr, sizeof (errstr) - 1);
                fprintf (stderr, "cannot write sndfile (%s)\n", errstr);
                fprintf(stderr, "%sWe done goofed...", HL_ERROR_PREFIX);
                exit(1);
            }
        }
    }
    sf_close(file);
    this->controller->removeBuffer(this->rb);
}

void Record::stop()
{
    this->endRecord.store(true);

    if (recordThread.joinable())
        recordThread.join();
}

vector<std::string> Record::getExportPaths()
{
    vector<std::string> path_copy = exportPaths;
    exportPaths.clear();

    return path_copy;
}

Record::~Record()
{
    delete rb;
}