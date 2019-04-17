#ifndef HL_LINUX_AUDIO_H
#define HL_LINUX_AUDIO_H

#include <stdlib.h>
#include <cstdint>
#include <thread>
#include <vector>

#include "hlaudio/internal/Device.h"
#include "hlaudio/internal/OSAudio.h"

#define HL_LINUX_FRAMES_PER_BUFFER 512

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

            std::vector<Device *> parsePulseAudioDevices();

        public:
            LinuxAudio();
            ~LinuxAudio();

            void capture();

            bool setActiveInputDevice(Device *device);
            static void startPAVUControl();

            std::vector<Device *> getDevices(DeviceType type);

            bool checkDeviceParams(Device *device);
    };
}

#endif // HL_LINUX_AUDIO_H