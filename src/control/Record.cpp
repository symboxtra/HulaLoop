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
    this->rb = this->controller->createBuffer(0.5);
}

/**
 * @brief Starts the capture of audio data by adding ringbuffer to Controller
 * and reading from ringbuffer
 *
 */
void Record::start()
{
    this->endRecord.store(false);
    recordThread = std::thread(&Record::recorder, this);

    this->controller->addBuffer(this->rb);
}

void Record::recorder()
{
    ring_buffer_size_t samplesRead;

    // Initialize libsndfile info.
    SF_INFO sfinfo = {0};
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.channels = NUM_CHANNELS;
    sfinfo.format = SF_FORMAT_FLAC | SF_FORMAT_PCM_24;

    // Create a timestamped file name
    char timestamp[20];
    time_t now = time(0);
    strftime(timestamp, 20, "%Y-%m-%d_%H-%M-%S", localtime(&now));
    std::string file_path = Export::getTempPath() + "/hulaloop_" + std::string(timestamp) + ".flac";
    SNDFILE *file = sf_open(file_path.c_str(), SFM_WRITE, &sfinfo);

    // Add file_path to vector of files
    exportPaths.push_back(file_path);

    // Keep recording until recording is stopped
    while (!this->endRecord.load())
    {
        void *ptr[2] = {0};
        ring_buffer_size_t sizes[2] = {0};
        samplesRead = this->rb->directRead(512, ptr + 0, sizes + 0, ptr + 1, sizes + 1);

        if (samplesRead > 0)
        {
            for (int i = 0; i < 2 && ptr[i] != nullptr; i++)
            {
                sf_count_t samplesWritten = sf_write_float(file, (float *)ptr[i], sizes[i]);
                if (samplesWritten != sizes[i])
                {
                    char errstr[256];
                    sf_error_str (0, errstr, sizeof (errstr) - 1);
                    hlDebugf("Could not write sndfile (%s)\n", errstr);
                    exit(1);
                }
            }
        }
    }


    sf_close(file);
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
    {
        recordThread.join();
    }
}

/**
 * @brief Get list of all files that contains captured audio in the current recording session
 *
 * @return std::vector<std::string> Vector of strings that represents file path to captured audio
 */
std::vector<std::string> Record::getExportPaths()
{
    return exportPaths;
}

/**
 * @brief Clear the vector to denote that the captured data has been discarded or
 * exported to a new file
 *
 */
void Record::clearExportPaths()
{
    exportPaths.clear();
}

/**
 * @brief Delete instance of the ringbuffer used
 *
 */
Record::~Record()
{
    hlDebugf("Record destructor called\n");

    stop();

    delete rb;
}