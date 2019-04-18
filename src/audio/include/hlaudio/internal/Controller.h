#ifndef HL_CONTROLLER_H
#define HL_CONTROLLER_H

#include <vector>

#include "Device.h"
#include "OSAudio.h"
#include "HulaRingBuffer.h"
#include "ICallback.h"

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

            // Callback Functionality
            void addCallback(ICallback* obj);
            void removeCallback(ICallback* obj);

            void startPlayback();
            void endPlayback();

            // Ringbuffer Functionality
            void copyToBuffers(const float *samples, ring_buffer_size_t sampleCount);
            ring_buffer_size_t playbackCopyToBuffers(const float *samples, ring_buffer_size_t sampleCount);

            std::vector<Device *> getDevices(DeviceType type) const;
            Device * findDeviceByName(const std::string &name) const;
            Device * findDeviceByName(const std::string &name, DeviceType type) const;

            bool setActiveInputDevice(Device *device) const;
            bool setActiveOutputDevice(Device *device) const;
    };
}

#endif // END HL_CONTROLLER_H
