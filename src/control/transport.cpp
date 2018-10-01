#include "transport.h"

#include <iostream>

Transport::Transport()
{

}

bool Transport::record()
{
    std::cout << "Record button clicked!" << std::endl;
    state = RECORDING;
    return true;
}

bool Transport::stop()
{
    std::cout << "Stop button clicked!" << std::endl;
    state = STOPPED;
    return true;
}

bool Transport::play()
{
    std::cout << "Play button clicked!" << std::endl;
    state = PLAYING;
    return true;
}

bool Transport::pause()
{
    std::cout << "Pause button clicked!" << std::endl;
    state = PAUSED;
    return true;
}

TransportState Transport::getState() const
{
    return state;
}

std::string Transport::stateToStr(const TransportState state) const
{

    switch(state)
    {

        case RECORDING:
            return "Recording";
        case STOPPED:
            return "Stopped";
        case PLAYING:
            return "Playing";
        case PAUSED:
            return "Paused";
        default:
            return "Unknown";

    }

}