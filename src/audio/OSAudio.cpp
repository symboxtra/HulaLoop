#include <algorithm>
#include <iostream>

#include "hlaudio/internal/HulaAudioError.h"
#include "hlaudio/internal/OSAudio.h"

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
 * Add upper layer functions to the callback list
 *
 * @param func Derived instance of ICallback class
 */
void OSAudio::addBufferReadyCallback(ICallback *func)
{
    // Check if callback function already exists
    if (find(callbackList.begin(), callbackList.end(), func) == callbackList.end())
    {
        this->callbackList.push_back(func);
    }
}

/**
 * Remove upper layer functions to the callback list
 *
 * @param func Derived instance of ICallback class
 */
void OSAudio::removeBufferReadyCallback(ICallback *callFunction)
{
    // Check if callback function exists to remove
    vector<ICallback *>::iterator it = find(callbackList.begin(), callbackList.end(), callFunction);
    if (it != callbackList.end())
    {
        this->callbackList.erase(it);
    }
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
    vector<HulaRingBuffer *>::iterator it = find(rbs.begin(), rbs.end(), rb);
    if (it != rbs.end())
    {
        this->rbs.erase(it);
    }

    // Stop the capture thread if there are no buffers left
    if (rbs.size() == 0)
    {
        // Join all threads
        // Threads should end since they checks the size of rbs
        for (auto &t : inThreads)
        {
            t.join();
            t.~thread();
        }
        inThreads.clear();
    }
}

/**
 * Write to each of the buffers contained in rbs.
 */
void OSAudio::copyToBuffers(const void *data, uint32_t bytes)
{
    SAMPLE *samples = (SAMPLE *)data;
    uint32_t sampleCount = BYTES_TO_SAMPLES(bytes);

    vector<HulaRingBuffer *>::iterator it;
    for (it = rbs.begin(); it != rbs.end(); it++)
    {
        (*it)->write(samples, sampleCount);
    }
}

/**
* Static function to allow starting a thread with an instance's capture method.
* This will block, so it should be called in a new thread.
*
* Ex: std::thread(&backgroundCapture, this);
*
* @param _this Instance of OSAudio subclass which capture should be called on.
*/
void OSAudio::backgroundCapture(OSAudio *_this)
{
    // Reset the thread interrupt flag
    _this->deviceSwitch = false;
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
    this->activeInputDevice = device;

    // Set the flag to signal thread end
    this->deviceSwitch = true;

    // Wait for all threads to catch the signal
    for (auto &t : inThreads)
    {
        t.join();
        t.~thread();
    }
    inThreads.clear();
}

/**
 * Virtual implementation of Destructor
 */
OSAudio::~OSAudio()
{
    printf("%sOSAudio destructor called\n", HL_PRINT_PREFIX);

    // Forcefully kill all in threads
    for (auto &t : inThreads)
    {
        t.detach();
        t.~thread();
    }
    inThreads.clear();
}