#ifndef HL_OSX_AUDIO_H
#define HL_OSX_AUDIO_H

#include <thread>
#include <vector>

#include <QCoreApplication>

#include "hlaudio/internal/Device.h"
#include "hlaudio/internal/OSAudio.h"

namespace hula
{
    /**
     * A audio class that captures system wide audio on OSX.
     */
    class OSXAudio : public OSAudio {

        Q_DECLARE_TR_FUNCTIONS(Audio)

        private:
            int daemonPID = -1;

        public:
            OSXAudio();
            virtual ~OSXAudio();

            int isDaemonRunning();
            void startDaemon();

            void capture();
            bool checkRates(Device *device);

            std::vector<Device *> getDevices(DeviceType type);

            static void test_capture(OSAudio *_this);
            void setActiveOutputDevice(Device *device);
    };
}

#endif // END HL_OSX_AUDIO_H
