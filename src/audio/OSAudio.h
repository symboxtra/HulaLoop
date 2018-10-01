#ifndef OS_AUDIO
#define OS_AUDIO

// System
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <thread>
#include <iostream>

#include "Device.h"
#include "ICallback.h"

using namespace std;

class OSAudio
{
    protected:
        Device* activeInputDevice;
        Device* activeOutputDevice;

        vector<ICallback*> callbackList;
        vector<thread> execThreads;

        uint32_t captureBufferSize;

    public:
        virtual ~OSAudio() = 0;

        void setBufferSize(uint32_t size);

        void addBufferReadyCallback(ICallback* c);
        void removeBufferReadyCallback(ICallback* func);

        virtual vector<Device*> getInputDevices() = 0;
        virtual vector<Device*> getOutputDevices() = 0;

        virtual void capture() = 0;

        void setActiveRecordDevice(Device* device); //TODO: Make virtual or maybe remove if can combine into one function
        virtual void setActiveOutputDevice(Device* device) = 0;
};

#endif