#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>

enum TransportState {
    RECORDING,
    STOPPED,
    PLAYING,
    PAUSED
};

class Transport
{
    private:
        TransportState state;

    public:
        Transport();

        bool record();
        bool stop();
        bool play();
        bool pause();

        TransportState getState() const;
        std::string stateToStr(const TransportState state) const;
};

#endif // TRANSPORT_H