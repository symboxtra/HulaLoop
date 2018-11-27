#ifndef HL_TRANSPORT_H
#define HL_TRANSPORT_H

#include <hlaudio/hlaudio.h>
#include <string>

#include <QCoreApplication>

#include "Record.h"

#define HL_INFINITE_RECORD -1

namespace hula
{
    /**
     * Available states for the recording/playback logic of the application.
     */
    enum TransportState
    {
        RECORDING,
        STOPPED,
        PLAYING,
        PAUSED
    };

    /**
     * @ingroup public_api
     *
     * Extra class for managing the state of the application and all audio related processes.
     */
    class Transport {

        Q_DECLARE_TR_FUNCTIONS(Transport)

        private:
            TransportState state;
            bool recordState;
            bool playbackState;
            bool initRecordClicked;

        protected:
            Record *recorder;

        public:
            Controller *controller;

        public:
            Transport();
            virtual ~Transport();

            bool record(double delay, double duration);
            bool record();
            bool stop();
            bool play();
            bool pause();

            Controller *getController() const;

            void exportFile(std::string targetDirectory);
            void deleteTempFiles();

            TransportState getState() const;
            std::string stateToStr(const TransportState state) const;
    };
}

#endif // HL_TRANSPORT_H