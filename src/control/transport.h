#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "Controller.h"
#include "Record.h"
#include <string>

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
 * Utility class for managing state of application and all audio related processes.
 */
class Transport {
    private:
        TransportState state;

    protected:
        Record* recorder;

    public:
        Controller* controller;

        Transport();
        ~Transport();

        bool record();
        bool stop();
        bool play();
        bool pause();

        TransportState getState() const;
        std::string stateToStr(const TransportState state) const;
};

#endif // TRANSPORT_H