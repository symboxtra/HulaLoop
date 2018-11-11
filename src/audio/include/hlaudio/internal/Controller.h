#ifndef HL_CONTROLLER_H
#define HL_CONTROLLER_H

#include <vector>

#include <QCoreApplication>

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

        Q_DECLARE_TR_FUNCTIONS(Controller)

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

            std::vector<Device *> getDevices(DeviceType type) const;

            void setActiveInputDevice(Device *device) const;
            void setActiveOutputDevice(Device *device) const;
    };
}

#endif // END HL_CONTROLLER_H
