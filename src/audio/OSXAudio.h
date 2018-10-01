#ifndef OSXAUDIO_H
#define OSXAUDIO_H

#include <thread>
#include <vector>

#include "OSAudio.h"
#include "Device.h"

using namespace std;

class OSXAudio : public OSAudio
{
    private:
        vector<Device *> getDevices(DeviceType type);

    public:
        OSXAudio();
        virtual ~OSXAudio();

        void capture();

        vector<Device *> getInputDevices();
        vector<Device *> getOutputDevices();

        static void test_capture(OSXAudio * param);
        void setActiveOutputDevice(Device * device);

};

#endif // END OSXAUDIO_H
