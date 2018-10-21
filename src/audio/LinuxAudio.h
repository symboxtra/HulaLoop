#include <alsa/asoundlib.h>

#include <stdlib.h>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

#include "hlaudio/internal/Device.h"
#include "hlaudio/internal/OSAudio.h"

#ifndef ALSA_PCM_NEW_HW_PARAMS_API
    #define ALSA_PCM_NEW_HW_PARAMS_API
#endif
#define FRAME_TIME 32
using namespace std;
using byte = uint8_t;

class LinuxAudio : public OSAudio {
    private:
        int bitrate;
        vector<Device *> iDevices;
        vector<Device *> oDevices;

    public:
        LinuxAudio();
        ~LinuxAudio();
        void capture();
        vector<Device *> getInputDevices();
        vector<Device *> getOutputDevices();
        vector<Device *> getDevices(DeviceType type);
        bool checkSamplingRate(Device *device);
        void clearDevices(vector<Device *> devices);
        static void test_capture(LinuxAudio *param);
        void setActiveOutputDevice(Device *device);

};
