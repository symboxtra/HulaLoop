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
            int daemonPID = -1;

        public:
            OSXAudio();
            virtual ~OSXAudio();

            pid_t isDaemonRunning();
            pid_t startDaemon();
            pid_t restartDaemon();

            void capture();
            bool checkRates(Device *device);

            std::vector<Device *> getDevices(DeviceType type);

            static void test_capture(OSAudio *_this);
            void setActiveOutputDevice(Device *device);
    };
}

#endif // END HL_OSX_AUDIO_H
