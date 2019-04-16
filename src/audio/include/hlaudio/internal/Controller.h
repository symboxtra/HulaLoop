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
    class Controller : public ICallback {

        private:
            OSAudio *audio;

            std::vector<ICallback *> callbackList;

        public:
            Controller();
            virtual ~Controller();

            void addBuffer(HulaRingBuffer *rb);
            void removeBuffer(HulaRingBuffer *rb);
            HulaRingBuffer *createBuffer(float duration);
            HulaRingBuffer *createAndAddBuffer(float duration);

            void startPlayback();
            void endPlayback();

            // Ringbuffer Functionality
            void copyToBuffers(const float *samples, ring_buffer_size_t sampleCount);
            ring_buffer_size_t playbackCopyToBuffers(const float *samples, ring_buffer_size_t sampleCount);

            // Callback Functionality
            void addBufferCallback(ICallback* func);
            void removeBufferCallback(ICallback* func);
            void handleData(const float *data, long size);

            std::vector<Device *> getDevices(DeviceType type) const;

            bool setActiveInputDevice(Device *device) const;
            bool setActiveOutputDevice(Device *device) const;
    };
}

#endif // END HL_CONTROLLER_H
