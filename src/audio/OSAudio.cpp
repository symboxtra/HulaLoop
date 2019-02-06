#include <algorithm>
#include <iostream>

#include "hlaudio/internal/HulaAudioError.h"
#include "hlaudio/internal/HulaAudioSettings.h"
#include "hlaudio/internal/OSAudio.h"

using namespace hula;

/**
 * Set the desired capture buffer size
 *
 * @param size Integer value representing the desired buffer size
 */
void OSAudio::setBufferSize(uint32_t size)
{
    this->captureBufferSize = size;
}

/**
 * Add an initialized buffer to the list of buffers that receive audio data.
 * If already present, the ring buffer will not be duplicated.
 *
 * @param rb HulaLoop ring buffer to add to the list.
*/
void OSAudio::addBuffer(HulaRingBuffer *rb)
{
    if (find(rbs.begin(), rbs.end(), rb) == rbs.end())
    {
        this->rbs.push_back(rb);
    }

    if (rbs.size() == 1 && endPlay.load())
    {
        // Signal death and join all threads
        this->endCapture.store(true);
        joinAndKillThreads(inThreads);

        // Start up the capture thread
        // TODO: Figure out how to manage this with playback
        this->endCapture.store(false);
        inThreads.emplace_back(std::thread(&backgroundCapture, this));
    }
}

/**
 * Remove a buffer from the list of buffers that receive audio data.
 * The removed buffer is not deleted and must be deleted by the user.
 *
 * This enables pausing retrieval when audio is not needed and
 * re-adding the same buffer when audio data is again needed.
 *
 * @param rb HulaLoop ring buffer to remove from the list.
*/
void OSAudio::removeBuffer(HulaRingBuffer *rb)
{
    std::vector<HulaRingBuffer *>::iterator it = find(rbs.begin(), rbs.end(), rb);
    if (it != rbs.end())
    {
        // Stop the capture thread if there will be no buffers left
        if (rbs.size() == 1)
        {
            // Signal death and join all threads
            this->endCapture.store(true);
            joinAndKillThreads(inThreads);
        }

        // Prevent invalid iterator for copyToBuffers
        this->rbs.erase(it);
    }
}

/**
 * Write to each of the buffers contained in rbs.
 *
 * @param samples Buffer of interleaved float samples
 * @param sampleCount Number of samples in the buffer
 */
void OSAudio::copyToBuffers(const float *samples, ring_buffer_size_t sampleCount)
{
    std::vector<HulaRingBuffer *>::iterator it;
    for (it = rbs.begin(); it != rbs.end(); it++)
    {
        (*it)->write(samples, sampleCount);
    }
}

/**
 * Write to the playback buffer plus all ring buffers.
 *
 * @param samples Buffer of interleaved float samples
 * @param sampleCount Number of samples in the buffer
 * @return Number of samples written to the playback buffer
 */
ring_buffer_size_t OSAudio::playbackCopyToBuffers(const float *samples, ring_buffer_size_t sampleCount)
{
    ring_buffer_size_t samplesWritten = this->playbackBuffer->write(samples, sampleCount);

    // Copy to the rest of the buffers
    copyToBuffers(samples, samplesWritten);

    return samplesWritten;
}

/**
* Static function to allow starting a thread with an instance's capture method.
* This will block, so it should be called in a new thread.
*
* Calling this directly outside of this class is dangerous.
* Any thread not in the inThreads vector cannot be guaranteed a valid endCapture
* signal since it won't be joined after a device switch or 0 buffer state.
* Sync flags might be reset before the independent thread sees them.
*
* @param _this Instance of OSAudio subclass which capture should be called on.
*/
void OSAudio::backgroundCapture(OSAudio *_this)
{
    // TODO: Does this need to move to setActiveInputDevice
    if (_this->rbs.size() == 0)
    {
        return;
    }

    if (_this->activeInputDevice == nullptr)
    {
        std::vector<Device *> devices = _this->getDevices((DeviceType)(DeviceType::RECORD | DeviceType::LOOPBACK));
        if (devices.empty())
        {
            return;
        }

        if (_this->activeInputDevice)
        {
            delete _this->activeInputDevice;
        }
        _this->activeInputDevice = new Device(*devices[0]);
        Device::deleteDevices(devices);
    }

    // Don't reset the endCapture flag here as it can cause a race condition
    _this->capture();
}

/**
* Static function to allow starting a thread with an instance's playback method.
* This will block, so it should be called in a new thread.
*
* Calling this directly outside of this class is dangerous.
* Any thread not in the outThreads vector cannot be guaranteed a valid endPlayback
* signal since it won't be joined after a device switch or 0 buffer state.
* Sync flags might be reset before the independent thread sees them.
*
* @param _this Instance of OSAudio subclass which capture should be called on.
*/
void OSAudio::backgroundPlayback(OSAudio *_this)
{

    if (_this->activeOutputDevice == NULL)
    {
        std::vector<Device *> devices = _this->getDevices((DeviceType)(DeviceType::PLAYBACK));
        if (devices.empty())
        {
            return;
        }

        if (_this->activeOutputDevice)
        {
            delete _this->activeOutputDevice;
        }
        _this->activeOutputDevice = new Device(*devices[0]);
        Device::deleteDevices(devices);
    }

    // Start the playback loop
    // DO NOT reset the interrupt flag here as it can cause a race condition
    _this->playback();
}

/**
 * Signal the start of the playback thread. Add playback buffer to the
 * HulaRingBuffer vector and start the playback thread. This signal is to notify
 * the backend to start reading data that will be played to the selected audio device.
 */
void OSAudio::startPlayback()
{
    // Kill any live record threads before starting audio capture
    this->endCapture.store(true);
    this->joinAndKillThreads(this->inThreads);

    this->endPlay.store(false);
    outThreads.emplace_back(std::thread(&backgroundPlayback, this));
}

/**
 * Signal the end of the playback thread. Kill all playback threads.
 * This signal is to notify the backend to stop reading
 * data to playback to the selected audio device.
 */
void OSAudio::endPlayback()
{
    this->endPlay.store(true);
    joinAndKillThreads(outThreads);

    this->playbackBuffer->clear();
}


/**
 * This routine will be called by the PortAudio engine when audio is needed.
 * It may be called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
*/
static int paPlayCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
    OSAudio *obj = (OSAudio *)userData;
    SAMPLE *wptr = (SAMPLE *)outputBuffer;

    // Prevent unused variable warnings.
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    ring_buffer_size_t elementsToRead = (ring_buffer_size_t)(framesPerBuffer * hula::HulaAudioSettings::getInstance()->getNumberOfChannels());

    SAMPLE *data1;
    SAMPLE *data2;
    ring_buffer_size_t size1;
    ring_buffer_size_t size2;
    ring_buffer_size_t samplesRead;

    samplesRead = obj->playbackBuffer->directRead(elementsToRead, (void **)&data1, &size1, (void **)&data2, &size2);

    // Do this by hand since we don't trust Windows memcpy
    for (ring_buffer_size_t i = 0; i < size1; i++)
    {
        wptr[i] = data1[i];
    }

    for (ring_buffer_size_t i = 0; i < size2; i++)
    {
        wptr[i + size1] = data2[i];
    }

    // Write silence if we couldn't get enough data
    if (samplesRead < elementsToRead)
    {
        hlDebug() << "Playback: Ring buffer underrun. Received " << samplesRead << " of " << elementsToRead << std::endl;
        hlDebug() << "Writing " << elementsToRead - samplesRead << " samples of silence." << std::endl;
        for (int i = 0; i < elementsToRead; i++)
        {
            wptr[i + size1 + size2] = 0;
        }
    }

    return paContinue;
}

/**
 * Generic playback function for all operating systems using PortAudio.
 *
 * If an OS needs a more specifc implementation, this can be overridden.
 */
void OSAudio::playback()
{
    PaStream *stream = {0};
    PaError err = paNoError;
    PaStreamParameters outputParameters = {0};

    if (this->activeOutputDevice->getID().portAudioID == -1)
    {
        hlDebug() << "Device did not have a PortAudio ID." << std::endl;
        return;
    }

    outputParameters.device = this->activeOutputDevice->getID().portAudioID;
    if (outputParameters.device == paNoDevice)
    {
        hlDebugf("Error: No default output device.\n");
        return;
    }

    outputParameters.channelCount = Pa_GetDeviceInfo(outputParameters.device)->maxOutputChannels;
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    hlDebug() << "Opening PortAudio playback stream..." << std::endl;

    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              paPlayCallback,
              this
    );

    if (err != paNoError)
    {
        hlDebug() << "Could not start stream" << std::endl;
        exit(1);
    }

    err = Pa_StartStream( stream );

    if (err != paNoError)
    {
        hlDebug() << "Could not start stream" << std::endl;
        exit(1);
    }

    while (!this->endPlay.load())
    {
        Pa_Sleep(100);
    }

    err = Pa_CloseStream(stream);

    if (err != paNoError)
    {
        hlDebug() << "Failed to close playback stream." << std::endl;
    }
}

/**
 * Set the selected input device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 * @return Success of device switch
 */
bool OSAudio::setActiveInputDevice(Device *device)
{
    // TODO: Handle error
    if (device == nullptr)
    {
        return false;
    }

    // If this isn't a loopback or record device
    if (!(device->getType() & LOOPBACK || device->getType() & RECORD))
    {
        return false;
    }

    try
    {
        if (!this->checkDeviceParams(device))
        {
            return false;
        }
    }
    catch(const AudioException &ae)
    {
        throw;
        return false;
    }

    if (this->activeInputDevice)
    {
        delete this->activeInputDevice;
    }
    this->activeInputDevice = new Device(*device);

    // Signal death and wait for all threads to catch the signal
    this->endCapture.store(true);
    joinAndKillThreads(inThreads);

    // Startup a new thread
    // Make sure a playback is not running
    if (endPlay.load())
    {
        this->endCapture.store(false);
        inThreads.emplace_back(std::thread(&backgroundCapture, this));
    }

    return true;
}

/**
 * Helper function to join and kill a vector of threads.
 * Will only join if joinable.
 *
 * @param threads Vector of threads to join and kill.
 */
void OSAudio::joinAndKillThreads(std::vector<std::thread> &threads)
{
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    threads.clear();
}

/**
 * Set the selected output device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 * @return Success of device switch
 */
bool OSAudio::setActiveOutputDevice(Device *device)
{
    // TODO: Handle error
    if (device == nullptr)
    {
        return false;
    }

    // If this isn't a loopback or record device
    if (!(device->getType() & PLAYBACK))
    {
        return false;
    }

    try
    {
        if (!this->checkDeviceParams(device))
        {
            return false;
        }
    }
    catch(const AudioException &ae)
    {
        throw;
        return false;
    }

    if (this->activeOutputDevice)
    {
        delete this->activeOutputDevice;
    }
    this->activeOutputDevice = new Device(*device);

    bool fromPlay = !endPlay.load();
    this->endPlay.store(true);
    joinAndKillThreads(outThreads);

    // Startup a new thread
    // Only restart the thread if we were just in a playback
    if (fromPlay && endCapture.load())
    {
        endPlay.store(false);
        outThreads.emplace_back(std::thread(&backgroundPlayback, this));
    }

    return true;
}

/**
 * Virtual implementation of Destructor
 */
OSAudio::~OSAudio()
{
    hlDebugf("OSAudio destructor called\n");

    // Signal thread death
    this->endCapture.store(true);
    this->endPlay.store(true);
    joinAndKillThreads(inThreads);
    joinAndKillThreads(outThreads);

    if (activeInputDevice)
    {
        delete activeInputDevice;
    }

    if (activeOutputDevice)
    {
        delete activeOutputDevice;
    }

    if (playbackBuffer)
    {
        delete playbackBuffer;
    }
}
