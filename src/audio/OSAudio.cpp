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
        this->rbs.erase(it);
    }

    // Stop the capture thread if there are no buffers left
    if (rbs.size() == 0)
    {
        // Signal death and join all threads
        this->endCapture.store(true);
        joinAndKillThreads(inThreads);
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

    if (_this->activeInputDevice == NULL)
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

    // Reset the thread interrupt flag
    _this->endCapture.store(false);
    _this->capture();
}

/**
 * Set the selected input device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
void OSAudio::setActiveInputDevice(Device *device)
{
    // TODO: Handle error
    if (device == NULL)
    {
        return;
    }

    // If this isn't a loopback or record device
    if (!(device->getType() & LOOPBACK || device->getType() & RECORD))
    {
        return;
    }

    this->checkRates(device);

    if (this->activeInputDevice)
    {
        delete this->activeInputDevice;
    }
    this->activeInputDevice = new Device(*device);

    // Signal death and wait for all threads to catch the signal
    this->endCapture.store(true);
    joinAndKillThreads(inThreads);

    // Startup a new thread
    inThreads.emplace_back(std::thread(&backgroundCapture, this));
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
 */
void OSAudio::setActiveOutputDevice(Device *device)
{
    this->checkRates(device);


    this->activeOutputDevice = device;
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
}
