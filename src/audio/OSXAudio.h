#ifndef HL_OSX_AUDIO_H
#define HL_OSX_AUDIO_H

#include <sys/types.h>

#include <thread>
#include <vector>

#include "hlaudio/internal/Device.h"
#include "hlaudio/internal/OSAudio.h"

namespace hula
{
    /**
     * A audio class that captures system wide audio on OSX.
     */
    class OSXAudio : public OSAudio {

        private:

        public:
            OSXAudio();
            virtual ~OSXAudio();

            pid_t isDaemonRunning();
            pid_t startDaemon();
            pid_t restartDaemon();

            void capture();
            void playback();
            bool checkDeviceParams(Device *device);

            std::vector<Device *> getDevices(DeviceType type);
    };
}

#endif // END HL_OSX_AUDIO_H
