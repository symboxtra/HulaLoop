#include <alsa/asoundlib.h>

#include <iostream>
#include <vector>
#include <thread>
#include "stdlib.h"
#include <cstdint>

#include "OSAudio.h"
#include "Device.h"

#ifndef ALSA_PCM_NEW_HW_PARAMS_API
#define ALSA_PCM_NEW_HW_PARAMS_API
#endif
#define FRAME_TIME 32
using namespace std;

class LinuxAudio : public OSAudio
{
    private:
        int bitrate;
    
    public:
        LinuxAudio();
        ~LinuxAudio();
        void capture(int seconds);
        vector<Device*> getInputDevices();
        vector<Device*> getOutputDevices();
        static void test_capture(LinuxAudio* param);
        void setActiveOutputDevice(Device* device);

};
