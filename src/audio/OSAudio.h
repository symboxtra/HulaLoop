#ifndef OS_AUDIO
#define OS_AUDIO

#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <thread>
#include <iostream>
#include <assert.h>
#include <chrono>
#include <future>

#include <map>


#include "Device.h"
#include "iCallback.h"

using byte = uint8_t;
using f_int_t = int(*)(uint32_t, byte*);

using namespace std;

class OSAudio
{

    protected:
        Device* activeInputDevice;
        Device* activeOutputDevice;

        //IMMDevice* testOutput;
        uint32_t captureBufferSize;

        vector<iCallback*> callbackList;

        vector<Device*> deviceList;
        vector<thread> execThreads;

    public:
        virtual ~OSAudio() = 0;

        void setBufferSize(uint32_t size);

        void addBufferReadyCallback(iCallback* c);
        void removeBufferReadyCallback(iCallback* func);

        virtual vector<Device*> getInputDevices() = 0;
        virtual vector<Device*> getOutputDevices() = 0;

        virtual void capture() = 0;

        void setActiveRecordDevice(Device* device);
        virtual void setActiveOutputDevice(Device* device) = 0;

};

#endif