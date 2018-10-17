#include <algorithm>
#include <iostream>

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

/*
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
}

/*
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
}

/*
 * Write to each of the buffers contained in rbs.
 */
void OSAudio::copyToBuffers(void *data, uint32_t bytes)
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
 * Set the selected input device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
void OSAudio::setActiveRecordDevice(Device *device)
{
    this->activeInputDevice = device;
}

/**
 * Virtual implementation of Destructor
 */
OSAudio::~OSAudio()
{
}