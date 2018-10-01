#ifndef OS_AUDIO
#define OS_AUDIO

// System
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <thread>
#include <iostream>

#include "Device.h"
#include "iCallback.h"

using namespace std;

class OSAudio
{
    protected:
        Device* activeInputDevice;
        Device* activeOutputDevice;

        vector<iCallback*> callbackList;
        vector<thread> execThreads;

        uint32_t captureBufferSize;

    public:
        virtual ~OSAudio() = 0;

        void setBufferSize(uint32_t size);

        void addBufferReadyCallback(iCallback* c);
        void removeBufferReadyCallback(iCallback* func);

        virtual vector<Device*> getInputDevices() = 0;
        virtual vector<Device*> getOutputDevices() = 0;

        virtual void capture() = 0;

        void setActiveRecordDevice(Device* device); //TODO: Make virtual or maybe remove if can combine into one function
        virtual void setActiveOutputDevice(Device* device) = 0;
};

#endif