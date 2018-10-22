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

/**
 * Set the selected input device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
void OSAudio::setActiveInputDevice(Device *device)
{
    this->checkRates(device);
    this->activeInputDevice = device;
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
}