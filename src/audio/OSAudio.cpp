#include <algorithm>
#include <iostream>

#include "hlaudio/internal/HulaAudioError.h"
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

    if (rbs.size() == 1)
    {
        // Signal death and join all threads
        this->endCapture.store(true);
        joinAndKillThreads(inThreads);

        // Start up the capture thread
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
 */
void OSAudio::copyToBuffers(const void *data, uint32_t bytes)
{
    SAMPLE *samples = (SAMPLE *)data;
    uint32_t sampleCount = BYTES_TO_SAMPLES(bytes);

    std::vector<HulaRingBuffer *>::iterator it;
    for (it = rbs.begin(); it != rbs.end(); it++)
    {
        (*it)->write(samples, sampleCount);
    }
}

/**
* Static function to allow starting a thread with an instance's capture method.
* This will block, so it should be called in a new thread.
*
* Calling this directly outside of this class is dangerous.
* Any thread not in the inThreads vector cannot be guarenteed a valid endCapture
* signal since it won't be joined after a device switch or 0 buffer state.
* Sync flags might be reset before the independent thread sees them.
*
* @param _this Instance of OSAudio subclass which capture should be called on.
*/
void OSAudio::backgroundCapture(OSAudio *_this)
{
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

    // Start the capture
    // Don't reset the endCapture flag here as it can cause a race condition
    _this->capture();
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
    this->endCapture.store(false);
    inThreads.emplace_back(std::thread(&backgroundCapture, this));

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
}
