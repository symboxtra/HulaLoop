#include "OSAudio.h"

void OSAudio::setBufferSize(uint32_t size)
{
    this->captureBufferSize = size;
}

void OSAudio::removeBufferReadyCallback(f_int_t callFunction)
{
    if(find(callbackList.begin(), callbackList.end(), callFunction) != callbackList.end())
        this->callbackList.erase(remove(callbackList.begin(), callbackList.end(), callFunction), callbackList.end());
}

void OSAudio::setActiveRecordDevice(Device* device)
{
}