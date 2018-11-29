#ifndef HL_LINUX_AUDIO_H
#define HL_LINUX_AUDIO_H

#include <alsa/asoundlib.h>

#include <stdlib.h>
#include <cstdint>
#include <thread>
#include <vector>

#include "hlaudio/internal/Device.h"
#include "hlaudio/internal/OSAudio.h"

#ifndef ALSA_PCM_NEW_HW_PARAMS_API
    #define ALSA_PCM_NEW_HW_PARAMS_API
#endif

#define FRAME_TIME 512

namespace hula
{
    /**
     * An audio class that captures system wide audio on Linux.
     */
    class LinuxAudio : public OSAudio {

        private:
            int bitrate;
            std::vector<Device *> iDevices;
            std::vector<Device *> oDevices;

        public:
            LinuxAudio();
            ~LinuxAudio();

            void capture();
            void playback();

            bool setActiveInputDevice(Device *device);
            static void startPAVUControl();

            std::vector<Device *> getDevices(DeviceType type);

            bool checkDeviceParams(Device *device);
    };
}

#endif // HL_LINUX_AUDIO_H