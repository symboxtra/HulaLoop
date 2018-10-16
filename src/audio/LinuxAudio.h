#include <alsa/asoundlib.h>

#include <stdlib.h>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

#include "hlaudio/internal/OSAudio.h"
#include "hlaudio/internal/Device.h"

#ifndef ALSA_PCM_NEW_HW_PARAMS_API
    #define ALSA_PCM_NEW_HW_PARAMS_API
#endif
#define FRAME_TIME 32
using namespace std;
using byte = uint8_t;

class LinuxAudio : public OSAudio {
    private:
        int bitrate;

    public:
        LinuxAudio();
        ~LinuxAudio();
        void capture();
        vector<Device *> getInputDevices();
        vector<Device *> getOutputDevices();
        static void test_capture(LinuxAudio *param);
        void setActiveOutputDevice(Device *device);

};
