#include "OSAudio.h"

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
 * @param func Derived instance of iCallback class
 */
void OSAudio::addBufferReadyCallback(iCallback* func)
{
    // Check if callback function already exists
    if(find(callbackList.begin(), callbackList.end(), func) == callbackList.end())
        this->callbackList.push_back(func);
}

/**
 * Remove upper layer functions to the callback list
 *
 * @param func Derived instance of iCallback class
 */
void OSAudio::removeBufferReadyCallback(iCallback* callFunction)
{
    // Check if callback function exists to remove
    if(find(callbackList.begin(), callbackList.end(), callFunction) != callbackList.end())
        this->callbackList.erase(remove(callbackList.begin(), callbackList.end(), callFunction), callbackList.end());
}

/**
 * Set the selected input device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
void OSAudio::setActiveRecordDevice(Device* device)
{
    this->activeInputDevice = device;
}

OSAudio::~OSAudio()
{
}