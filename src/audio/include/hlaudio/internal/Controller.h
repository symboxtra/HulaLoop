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
    protected:
        OSAudio *audio;

        vector<ICallback *> callbackList;

    public:
        Controller();
        ~Controller();

        void addBufferReadyCallback(ICallback *func);
        void removeBufferReadyCallback(ICallback *func);

        void addBuffer(HulaRingBuffer *rb);
        void removeBuffer(HulaRingBuffer *rb);
        HulaRingBuffer * createBuffer(float duration);
        HulaRingBuffer * createAndAddBuffer(float duration);

        void handleData(uint8_t *data, uint32_t size);
        vector<Device *> getInputDevices();
        vector<Device *> getOutputDevices();
};

#endif