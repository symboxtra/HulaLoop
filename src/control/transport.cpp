#include <iostream>

#include "transport.h"

/**
 * Construct a new instance of the Transport class.
 */
Transport::Transport()
{
    controller = new Controller();
}

/**
 * Start and handle the process of recording.
 */
bool Transport::record()
{
    std::cout << "Record button clicked!" << std::endl;
    state = RECORDING;
    return true;
}

/**
 * Enter the stopped state for playback or recording.
 */
bool Transport::stop()
{
    std::cout << "Stop button clicked!" << std::endl;
    state = STOPPED;
    return true;
}

/**
 * Playback previously recorded audio.
 */
bool Transport::play()
{
    std::cout << "Play button clicked!" << std::endl;
    state = PLAYING;
    return true;
}

/**
 * Enter the paused state for playback or recording.
 */
bool Transport::pause()
{
    std::cout << "Pause button clicked!" << std::endl;
    state = PAUSED;
    return true;
}

/**
 * Return the current state of the Transport object.
 *
 * @return state Current transport state.
 */
TransportState Transport::getState() const
{
    return state;
}

/**
 * Convert an enum of type TransportState to a presentable string.
 *
 * @return stateString String containing appropriate state descriptor.
 */
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

/**
 * Delete the controller we created
 */
Transport::~Transport()
{
    delete controller;
}