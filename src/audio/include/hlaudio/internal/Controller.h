#ifndef CONTROL
#define CONTROL

#include <iostream>
#include <vector>

#include "Device.h"
#include "OSAudio.h"
#include "HulaRingBuffer.h"

// TODO: Add public description of class
/**
 * A class that structures the receival of audio from the OS framework
 */
class Controller {
    private:
        OSAudio *audio;

    public:
        #ifndef NDEBUG
        Controller(bool dryRun);
        #endif // END NDEBUG

        Controller();
        virtual ~Controller();

        void addBuffer(HulaRingBuffer *rb);
        void removeBuffer(HulaRingBuffer *rb);
        HulaRingBuffer *createBuffer(float duration);
        HulaRingBuffer *createAndAddBuffer(float duration);

        void handleData(uint8_t *data, uint32_t size);
        vector<Device *> getInputDevices() const;
        vector<Device *> getOutputDevices() const;
        vector<Device *> getDevices(DeviceType type) const;

        void setActiveInputDevice(Device *device) const;
        void setActiveOutputDevice(Device *device) const;
};

#endif
