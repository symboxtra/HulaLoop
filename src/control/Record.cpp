#include "hlcontrol/internal/Record.h"

#include "hlcontrol/internal/Export.h"
#include <iostream>
#include <fstream>
#include <sndfile.h>

Record::Record(Controller* control)
{
    this->controller = control;
    this->rb = NULL;

    //cout << "Temp Path: " << Export::getTempPath() << endl;
}

void Record::start()
{
    if(this->rb == NULL)
        this->rb = this->controller->createAndAddBuffer(0.5);

    this->endRecord.store(false);
    recordThread = thread(&Record::recorder, this);
}

void Record::recorder()
{
    uint32_t samplesRead;
    float buffer[512];

    SF_INFO sfinfo;
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.channels = NUM_CHANNELS;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    // Open the file write-only
    std::string file_path = Export::getTempPath() + "/time_stamped.wav";
    std::string file_path2 = Export::getTempPath() + "/time_stamped2.wav";
    SNDFILE* file = sf_open(file_path.c_str(), SFM_WRITE, &sfinfo);

    FILE* fd = fopen(file_path2.c_str(), "w");

    while(!this->endRecord.load())
    {
        samplesRead = this->rb->read(buffer, 512);

        if(samplesRead > 0)
        {
            printf("Address of read data: %p\n", buffer);

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
            fwrite(buffer, sizeof(float), samplesRead, fd);
        }
    }
    fclose(fd);
    //myfile.close();
    sf_close(file);
}

void Record::stop()
{
    this->endRecord.store(true);

    if(recordThread.joinable())
        recordThread.join();

    this->controller->removeBuffer(this->rb);
}

Record::~Record()
{
    delete rb;
}