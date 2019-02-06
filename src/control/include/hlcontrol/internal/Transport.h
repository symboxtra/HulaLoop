#ifndef HL_TRANSPORT_H
#define HL_TRANSPORT_H

#include <hlaudio/hlaudio.h>
#include <string>

#include <QCoreApplication>

#include "Record.h"
#include "Playback.h"

#define HL_INFINITE_RECORD -1
#define HL_TRANSPORT_LOCKOUT_MS 200

namespace hula
{
    /**
     * Available states for the recording/playback logic of the application.
     */
    enum TransportState
    {
        READY,
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
            bool canRecord;
            bool canPlayback;
            bool initRecordClicked;

        protected:
            /**
             * Instance of the Recorder class.
             */
            Record *recorder;

            /**
             * Instance of the Playback class.
             */
            Playback *player;

        public:
            /**
             * Instance of the Controller class.
             */
            Controller *controller;

        public:
            Transport();
            virtual ~Transport();

            bool record(double delay, double duration);
            bool record();
            bool stop();
            bool play();
            bool pause();
            void discard();

            Controller *getController() const;

            void exportFile(std::string targetDirectory);

            bool hasExportPaths();

            TransportState getState() const;
            std::string stateToStr(const TransportState state) const;
    };
}

#endif // HL_TRANSPORT_H