#ifndef HL_CONTROLLER_H
#define HL_CONTROLLER_H

#include <vector>

#include "Device.h"
#include "OSAudio.h"
#include "HulaRingBuffer.h"

namespace hula
{
    /**
     * @ingroup public_api
     *
     * Central component of the audio backend.
     * A class that structures the receival of audio from the OS framework.
     */
    class Controller {

        private:
            OSAudio *audio;

        public:
            Controller();
            virtual ~Controller();

            void addBuffer(HulaRingBuffer *rb);
            void removeBuffer(HulaRingBuffer *rb);
            HulaRingBuffer *createBuffer(float duration);
            HulaRingBuffer *createAndAddBuffer(float duration);

            void startPlayback();
            void endPlayback();
            void copyToBuffers(const void *data, uint32_t bytes);

            std::vector<Device *> getDevices(DeviceType type) const;

            bool setActiveInputDevice(Device *device) const;
            bool setActiveOutputDevice(Device *device) const;
    };
}

#endif // END HL_CONTROLLER_H
