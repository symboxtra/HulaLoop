#ifndef OSXAUDIO_H
#define OSXAUDIO_H

#include <thread>
#include <vector>

#include "hlaudio/internal/Device.h"
#include "hlaudio/internal/OSAudio.h"

using namespace std;

/**
 * A audio class that captures system wide audio on OSX
 */
class OSXAudio : public OSAudio {
    private:
        vector<Device *> getDevices(DeviceType type);

    public:
        OSXAudio();
        virtual ~OSXAudio();

        void capture();

        bool checkRates(Device *device);
        vector<Device *> getInputDevices();
        vector<Device *> getOutputDevices();

        static void test_capture(OSXAudio *param);
        void setActiveOutputDevice(Device *device);

};

#endif // END OSXAUDIO_H
