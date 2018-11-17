#include "hlcontrol/internal/Record.h"

#include "hlcontrol/internal/Export.h"
#include <iostream>
#include <fstream>
#include <sndfile.h>

using namespace hula;

/**
 * @brief Construct a new Record to capture audio data and store in temp file
 *
 * @param control Controller instance that will be used to interact with the ringbuffer
 */
Record::Record(Controller *control)
{
    this->controller = control;
    this->rb = NULL;
}

/**
 * @brief Starts the capture of audio data by adding ringbuffer to Controller
 * and reading from ringbuffer
 *
 */
void Record::start()
{
    if (this->rb == NULL)
        this->rb = this->controller->createBuffer(0.5);

    this->endRecord.store(false);
    recordThread = thread(&Record::recorder, this);
}

void Record::recorder()
{
    ring_buffer_size_t samplesRead;
    float buffer[44100];

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

    // TODO: Remove once debugged
    //std::string temp = Export::getTempPath() + "/hulaloop_log1.txt";
    //std::ofstream tempfile(temp.c_str(), ios::binary);


    // Add file_path to vector of files
    exportPaths.push_back(file_path);

    // Add ringbuffer to buffer list
    this->controller->addBuffer(this->rb);

    // Keep recording until recording is stopped
    while (!this->endRecord.load())
    {
        void* ptr[2] = {0};
        ring_buffer_size_t sizes[2] = {0};
        samplesRead = this->rb->directRead(512, ptr + 0, sizes + 0, ptr + 1, sizes + 1);

        if (samplesRead > 0)
        {
            //for(int i = 0;i < samplesRead;i++)
            //{
            //    tempfile << buffer[i] << endl;
            //}
            printf("Samples read: %d\n", samplesRead);

            for(int i = 0;i < 2 && ptr[i] != NULL;i++)
            {
                sf_count_t samplesWritten = sf_write_float(file, (float*)ptr[i], sizes[i]);
                if (samplesWritten != sizes[i])
                {
                    char errstr[256];
                    sf_error_str (0, errstr, sizeof (errstr) - 1);
                    fprintf (stderr, "cannot write sndfile (%s)\n", errstr);
                    fprintf(stderr, "%sWe done goofed...", HL_ERROR_PREFIX);
                    exit(1);
                }
            }
        }
    }
    sf_close(file);
    //tempfile.close();
    this->controller->removeBuffer(this->rb);
}

/**
 * @brief Stops the capture of audio data by removing ringbuffer from Controller
 *
 */
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

/**
 * @brief Delete instance of the ringbuffer used
 *
 */
Record::~Record()
{
    delete rb;
}