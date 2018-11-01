#ifndef HL_LINUX_AUDIO
#define HL_LINUX_AUDIO

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
#define FRAME_TIME 8192
using namespace std;

class LinuxAudio : public OSAudio {
    private:
        int bitrate;
        vector<Device *> iDevices;
        vector<Device *> oDevices;

    public:
        LinuxAudio();
        ~LinuxAudio();
        void capture();

        static void startPAVUControl();

        vector<Device *> getDevices(DeviceType type);

        bool checkRates(Device *device);

        void setActiveOutputDevice(Device *device);

};

#endif // HL_LINUX_AUDIO