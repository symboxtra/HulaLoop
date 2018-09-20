#ifndef WIN_AUDIO
#define WIN_AUDIO

#include "Device.h"

#include <string>
#include <vector>

using byte = uint8_t; 

using f_int_t = int(*)(int);

class WindowsAudio {

private:
    Device* activeOutputDevice;
    Device* activeInputDevice;
    size_t captureBufferSize;
    vector<f_int_t> callbackList;


public:

    Device* getInputDevices();
    Device* getOutputDevices();

    void setBufferSize(size_t size);

    void setActiveRecordDevice(Device* device);
    void setActiveOutputDevice(Device* device);

    void addBufferReadyCallback(f_int_t callFunction);
};

#endif