#ifndef OSXAUDIO_H
#define OSXAUDIO_H

#include <thread>
#include <vector>

#include "hlaudio/internal/Device.h"
#include "hlaudio/internal/OSAudio.h"

#define PA_CHECK_ERROR if ( err != paNoError ) { goto done; }

/**
 * A audio class that captures system wide audio on OSX.
 */
class OSXAudio : public OSAudio {
    private:
        std::vector<Device *> getDevices(DeviceType type);

    public:
        OSXAudio();
        virtual ~OSXAudio();

        void capture();

        std::vector<Device *> getInputDevices();
        std::vector<Device *> getOutputDevices();

        static void test_capture(OSAudio *_this);
        void setActiveOutputDevice(Device *device);

};

#endif // END OSXAUDIO_H
