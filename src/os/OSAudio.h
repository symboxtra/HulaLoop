#ifndef OS_AUDIO
#define OS_AUDIO

#include <stdlib.h>
#include <algorithm>
#include <vector>

#include "Device.h"

using byte = uint8_t;
using f_int_t = int(*)(int);

using namespace std;

class OSAudio
{
    private:
        OSAudio();

    protected:
        Device* activeInputDevice;
        Device* activeOutputDevice;
        uint32_t captureBufferSize;

        vector<f_int_t> callbackList;

        vector<Device*> deviceList;

    public:
        void setBufferSize(uint32_t size);

        void addBufferReadyCallback(f_int_t func);
        void removeBufferReadyCallback(f_int_t func);
        
        vector<Device*> getAudioDevices();
        virtual vector<Device*> getInputDevices() = 0;
        virtual vector<Device*> getOutputDevices() = 0;

        virtual void capture() = 0;

        void setActiveRecordDevice(Device* device);
        void setActiveOutputDevice(Device* device);
         
};

#endif