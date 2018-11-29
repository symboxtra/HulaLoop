#include "hlcontrol/internal/Playback.h"

using namespace hula;

#include <iostream>
#include <sndfile.h>

/**
 * @brief Construct a new Playback instance to replay the captured audio data
 *
 * @param control Controller instance that will be used to interact with the ringbuffer
 */
Playback::Playback(Controller *control)
{
    this->controller = control;
}

/**
 * @brief Starts the playback of audio data from the specified time of the capture duration
 *
 * @param initTime Begin time of the capture duration (in seconds)
 */
void Playback::start(uint32_t startTime)
{
    this->endPlay.store(false);
    playThread = std::thread(&Playback::player, this);
}

void Playback::player()
{
    // Initialize libsndfile info.
    SF_INFO sfinfo;
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.channels = NUM_CHANNELS;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    float* buffer = new float[1024];

    // TODO: Math to determine which temp file and location in temp file
    std::string file_path = "C:\\Users\\patel\\Desktop\\test.wav";
    SNDFILE *file = sf_open(file_path.c_str(), SFM_READ, &sfinfo);

    this->controller->startPlayback();
    sf_count_t samplesRead = 0;
    while(!this->endPlay.load())
    {
        samplesRead = sf_read_float(file, buffer, 1024);
        this->controller->copyToBuffers(buffer, samplesRead * sizeof(float));
        std::cout << "Samples from file: " << samplesRead << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    sf_close(file);
    this->controller->endPlayback();
}

/**
 * @brief Stop the playback of audio data by removing ringbuffer from Controller
 *
 */
void Playback::stop()
{
    this->endPlay.store(true);

    if (playThread.joinable())
        playThread.join();
}

Playback::~Playback()
{

}