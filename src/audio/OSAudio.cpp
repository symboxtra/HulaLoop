#include "OSAudio.h"

void OSAudio::setBufferSize(uint32_t size)
{
    this->captureBufferSize = size;
}

void OSAudio::addBufferReadyCallback(iCallback* func)
{
    if(find(callbackList.begin(), callbackList.end(), func) == callbackList.end())
        this->callbackList.push_back(func);
}

void OSAudio::removeBufferReadyCallback(iCallback* callFunction)
{
    if(find(callbackList.begin(), callbackList.end(), callFunction) != callbackList.end())
        this->callbackList.erase(remove(callbackList.begin(), callbackList.end(), callFunction), callbackList.end());
}

void OSAudio::setActiveRecordDevice(Device* device)
{
}

OSAudio::~OSAudio()
{
}