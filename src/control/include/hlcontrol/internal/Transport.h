#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <hlaudio/hlaudio.h>
#include <string>

#include "Record.h"

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
    private:
        TransportState state;
        bool recordState = true;
        bool playbackState = false;

    protected:
        Record* recorder;

    public:
        Controller* controller;

    public:

        #ifndef NDEBUG
        Transport(bool dryRun);
        #endif // END NDEBUG

        Transport();
        virtual ~Transport();

        bool record();
        bool stop();
        bool play();
        bool pause();

        Controller *getController() const;

        void exportFile(std::string targetDirectory);

        TransportState getState() const;
        std::string stateToStr(const TransportState state) const;
};

#endif // TRANSPORT_H