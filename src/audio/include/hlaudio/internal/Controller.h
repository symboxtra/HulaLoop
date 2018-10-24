#ifndef CONTROL
#define CONTROL

#include <iostream>
#include <vector>

#include "Device.h"
#include "OSAudio.h"
#include "ICallback.h"
#include "HulaRingBuffer.h"

using byte = uint8_t;

// TODO: Add public description of class
/**
 * A class that structures the receival of audio from the OS framework
 */
class Controller : public ICallback {
    private:
        OSAudio *audio;

    protected:
        vector<ICallback *> callbackList;

    public:
        #ifndef NDEBUG
        Controller(bool dryRun);
        #endif // END NDEBUG

        Controller();
        virtual ~Controller();

        void addBufferReadyCallback(ICallback *func);
        void removeBufferReadyCallback(ICallback *func);

        void addBuffer(HulaRingBuffer *rb);
        void removeBuffer(HulaRingBuffer *rb);
        HulaRingBuffer * createBuffer(float duration);
        HulaRingBuffer * createAndAddBuffer(float duration);

        void handleData(uint8_t *data, uint32_t size);
        vector<Device *> getInputDevices();
        vector<Device *> getOutputDevices();
        void setActiveInputDevice(Device *device) const;
        void setActiveOutputDevice(Device *device) const;
};

#endif
