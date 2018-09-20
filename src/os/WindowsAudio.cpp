#include "WindowsAudio.h"

WindowsAudio::setBufferSize(size_t size)
{
    this->captureBufferSize = size;
}

WindowsAudio::addBufferReadyCallback(f_int_t callFunction)
{
    this->callbackList.push_back(callFunction);
}

WindowsAudio::getInputDevices()
{

}

WindowsAudio::getOutputDevices()
{
    
}

WindowsAudio::setActiveOutputDevice(Device* device)
{
    this->activeOutputDevice = device;
}

WindowsAudio::setActiveRecordDevice(Device* device)
{
    this->activeInputDevice = device;
}