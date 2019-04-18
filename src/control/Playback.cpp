#include "hlcontrol/internal/Playback.h"

using namespace hula;

#include <iostream>
#include <sndfile.h>

/**
 * @brief Construct a new Playback instance to replay the captured audio data
 *
 * @param control Controller instance that will be used to interact with the ringbuffer
 */
Playback::Playback(Controller *control, Record *record)
{
    this->controller = control;
    this->recorder = record;
}

/**
 * @brief Starts the playback of audio data from the specified time of the capture duration
 *
 * @param initTime Begin time of the capture duration (in seconds)
 */
void Playback::start(uint32_t startTime)
{
    // Make sure the last thread was joined or
    // the assignment of the new thread will fail
    this->endPlay.store(true);
    if (playThread.joinable())
        playThread.join();

    this->endPlay.store(false);
    playThread = std::thread(&Playback::player, this);
}

void Playback::player()
{
    this->controller->startPlayback();

    // Initialize libsndfile info.
    SF_INFO sfinfo;
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.channels = NUM_CHANNELS;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    ring_buffer_size_t maxSize = 1024;
    float buffer[1024];

    size_t fileIndex = 0;
    std::vector<std::string> files = recorder->getExportPaths();

    // No files to play
    if (files.size() == 0)
    {
        return;
    }

    hlDebug() << "Playing back " << files.size() << " files." << std::endl;

    SNDFILE *sndFile = sf_open(files[fileIndex].c_str(), SFM_READ, &sfinfo);

    hlDebug() << "Opened file #" << fileIndex << std::endl;
    hlDebug() << "Location: " << files[fileIndex] << std::endl;

    sf_count_t samplesRead = 0;
    while(!this->endPlay.load())
    {
        samplesRead = sf_read_float(sndFile, (float *)&buffer, maxSize);

        ring_buffer_size_t lastWrite = 0;
        ring_buffer_size_t totalWrite = 0;
        while (!this->endPlay.load() && totalWrite < samplesRead)
        {
            ring_buffer_size_t samplesWritten = this->controller->playbackCopyToBuffers((float *)&buffer + lastWrite, samplesRead - totalWrite);
            lastWrite = samplesWritten;
            totalWrite += samplesWritten;

            // If the buffer was too full, wait a little bit
            if (totalWrite != samplesRead)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
        }

        // We're done with the file
        if (samplesRead == 0)
        {
            // Open the next file or end the playback
            if (fileIndex < files.size() - 1)
            {
                sf_close(sndFile);

                fileIndex++;
                sndFile = sf_open(files[fileIndex].c_str(), SFM_READ, &sfinfo);

                hlDebug() << "Opened file #" << fileIndex << std::endl;
            }
            else
            {
                hlDebug() << "Played final file. Exiting playback loop." << std::endl;
                this->endPlay.store(true);

                // TODO: The UI will not know that this has ended
            }

            continue;
        }

        // Calculate the length of the audio that we just wrote
        // With a buffer of 1024 samples, 2 channels, and 44,100 Hz sample rate
        // this is approximately 11ms
        // We trim this by 3ms to accomodate for execution
        std::this_thread::sleep_for(std::chrono::milliseconds(maxSize / NUM_CHANNELS * 1000 / SAMPLE_RATE - 3));
    }

    hlDebug() << "Playback write loop exited." << std::endl;

    sf_close(sndFile);

    // Make sure the playback buffer gets drained first
    std::this_thread::sleep_for(std::chrono::milliseconds(HL_PLAYBACK_RB_DURATION * 1000 + 200));
    this->controller->endPlayback();
}

/**
 * @brief Stop the playback of audio data by removing ringbuffer from Controller
 *
 */
void Playback::stop()
{
    this->endPlay.store(true);
    this->controller->endPlayback();

    if (playThread.joinable())
        playThread.join();
}

Playback::~Playback()
{
    stop();
}