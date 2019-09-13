#include <algorithm>
#include <iostream>

#include "hlaudio/internal/HulaAudioError.h"
#include "hlaudio/internal/HulaAudioSettings.h"
#include "hlaudio/internal/OSAudio.h"

using namespace hula;

/**
 * Add an initialized buffer to the list of buffers that receive audio data.
 * If already present, the ring buffer will not be duplicated.
 *
 * @param rb HulaLoop ring buffer to add to the list.
*/
void OSAudio::addBuffer(HulaRingBuffer *rb)
{
    // Guard against NULL
    if (!rb)
    {
        return;
    }

    // Prevent duplicate buffers in list
    if (find(rbs.begin(), rbs.end(), rb) == rbs.end())
    {
        this->rbs.push_back(rb);

        // Start record thread if only one buffer or callback exists
        if (rbs.size() == 1 && cbs.size() == 0)
        {
            startRecord();
        }
    }
}

/**
 * Internal function for managing record thread creation
 */
void OSAudio::startRecord()
{
    hlDebug() << "OSAudio: Start record called." << std::endl;

    // Prevent other state changes
    this->stateSem.wait();
    if(this->endCapture.load() && this->endPlay.load())
    {
        // Mutex will be unlocked by backgroundCapture once
        // thread creation is complete
        inThreads.emplace_back(std::thread(&OSAudio::backgroundCapture, this));
    }
    else
    {
        hlDebug() << "OSAudio: Start record fell through." << std::endl;
        this->stateSem.notify();
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
        if (rbs.size() == 1 && cbs.size() == 0)
        {
            endRecord();
        }

        // Prevent invalid iterator for copyToBuffers
        this->rbs.erase(it);
    }
}

/**
 * Internal function for managing record thread deletion
 */
void OSAudio::endRecord()
{
    hlDebug() << "OSAudio: End record called." << std::endl;

    this->stateSem.wait();

    this->endCapture.store(true);
    joinAndKillThreads(inThreads);

    this->stateSem.notify();
}

/**
 * Add a callback to the list of callbacks that receive audio data.
 * If already present, the callback will not be duplicated.
 *
 * @param obj ICallback object to add to the callback list.
 */
void OSAudio::addCallback(ICallback *obj)
{
    // Guard against NULL
    if (!obj)
    {
        return;
    }

    if(std::find(cbs.begin(), cbs.end(), obj) == cbs.end())
    {
        this->cbs.push_back(obj);

        if(cbs.size() == 1 && rbs.size() == 0)
        {
            startRecord();
        }
    }
}

/**
 * Call each callback contained in cbs
 *
 * @param samples Audio data to be copied
 * @param sampleCount Number of samples
 */
void OSAudio::doCallbacks(const float *samples, ring_buffer_size_t sampleCount)
{
    std::vector<ICallback *>::iterator it;
    for (it = cbs.begin(); it != cbs.end(); it++)
    {
        (*it)->handleData(samples, sampleCount);
    }
}

/**
 * Remove a callback from the list of callbacks that receive audio data.
 *
 * @param obj ICallback object to add to the callback list.
 */
void OSAudio::removeCallback(ICallback *obj)
{
    // Check if callback function exists to remove
    std::vector<ICallback *>::iterator it = std::find(cbs.begin(), cbs.end(), obj);
    if(it != cbs.end())
    {
        // Stop the capture thread if there will be no buffers left
        if(cbs.size() == 1 && rbs.size() == 0)
        {
            endRecord();
        }

        this->cbs.erase(it);
    }
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
*/
void OSAudio::backgroundCapture()
{
    // TODO: Does this need to move to setActiveInputDevice
    if (this->rbs.size() == 0 && this->cbs.size() == 0)
    {
        this->stateSem.notify();
        return;
    }

    // Default to first device
    if (this->activeInputDevice == nullptr)
    {
        std::vector<Device *> devices = this->getDevices((DeviceType)(DeviceType::RECORD | DeviceType::LOOPBACK));
        if (devices.empty())
        {
            hlDebug() << "In backgroundCapture: No input devices available." << std::endl;

            this->stateSem.notify();
            return;
        }

        // TODO: Why? Not sure what this means since above condition is opposite
        // if (this->activeInputDevice)
        // {
        //     delete this->activeInputDevice;
        // }
        this->activeInputDevice = new Device(*devices[0]);
        Device::deleteDevices(devices);
    }

    this->endCapture.store(false);

    // TODO: Make sure this is a FIFO notification if necessary
    // Notify any waiting threads
    this->stateSem.notify();

    // Infinite loop until endRecord
    this->capture();
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
*/
void OSAudio::backgroundPlayback()
{
    // Default to first device
    if (this->activeOutputDevice == nullptr)
    {
        std::vector<Device *> devices = this->getDevices((DeviceType)(DeviceType::PLAYBACK));
        if (devices.empty())
        {
            hlDebug() << "In backgroundPlayback: No output devices available." << std::endl;

            this->stateSem.notify();
            return;
        }

        // TODO: Why? Not sure what this means since above condition is opposite
        // if (this->activeOutputDevice)
        // {
        //     delete this->activeOutputDevice;
        // }
        this->activeOutputDevice = new Device(*devices[0]);
        Device::deleteDevices(devices);
    }

    this->endPlay.store(false);

    // TODO: Make sure this is a FIFO notification if necessary
    // Notify any waiting threads
    this->stateSem.notify();

    // Infinite loop until endPlayback
    this->playback();
}

/**
 * Signal the start of the playback thread. Add playback buffer to the
 * HulaRingBuffer vector and start the playback thread. This signal is to notify
 * the backend to start reading data that will be played to the selected audio device.
 */
void OSAudio::startPlayback()
{
    hlDebug() << "OSAudio: Start playback called." << std::endl;


    this->stateSem.wait();

    if(this->endPlay.load() && this->endCapture.load())
    {
        // Mutex will be unlocked by backgroundPlayback once
        // thread creation is complete
        outThreads.emplace_back(std::thread(&OSAudio::backgroundPlayback, this));
    }
    else
    {
        hlDebug() << "OSAudio: Start playback fell through." << std::endl;
        this->stateSem.notify();
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
 * Signal the end of the playback thread. Kill all playback threads.
 * This signal is to notify the backend to stop reading
 * data to playback to the selected audio device.
 */
void OSAudio::endPlayback()
{
    hlDebug() << "OSAudio: End playback called." << std::endl;

    this->stateSem.wait();

    this->endPlay.store(true);
    joinAndKillThreads(outThreads);

    // Remove any unplayed audio
    this->playbackBuffer->clear();

    this->stateSem.notify();
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
        for (int i = samplesRead; i < elementsToRead; i++)
        {
            wptr[i] = 0;
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

    hlDebug() << "Closing PortAudio playback stream." << std::endl;

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
        throw AudioException(HL_INPUT_EQUAL_OUTPUT_CODE, HL_INPUT_EQUAL_OUTPUT_MSG);
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

    // Store value since endRecord will override endCapture
    bool prevRec = !endCapture.load();

    endRecord();

    // Startup a new thread
    // Only restart the thread if we were just in a playback
    if(prevRec)
    {
        startRecord();
    }

    return true;
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
        throw AudioException(HL_OUTPUT_EQUAL_INPUT_CODE, HL_OUTPUT_EQUAL_INPUT_MSG);
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

    // Store value since endPlayback will override endPlay
    bool prevPlay = !endPlay.load();

    endPlayback();

    // Startup a new thread
    // Only restart the thread if we were just in a playback
    if (prevPlay)
    {
        startPlayback();
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
