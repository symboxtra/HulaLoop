#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <hlaudio/hlaudio.h>
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
        Controller *controller;

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

        vector<Device *> getInputDevices() const;
        vector<Device *> getOutputDevices() const;
        Controller *getController() const;

        void exportFile(std::string targetDirectory);

        TransportState getState() const;
        std::string stateToStr(const TransportState state) const;
};

#endif // TRANSPORT_H