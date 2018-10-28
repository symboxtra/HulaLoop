#include <iostream>

#include "hlcontrol/internal/HulaControlError.h"
#include "hlcontrol/internal/Transport.h"

/**
 * Construct a new instance of the Transport class.
 */
Transport::Transport()
{
    controller = new Controller();
}

#ifndef NDEBUG
/**
 * ---------------- FOR TESTING/DEBUG BUILDS ONLY -----------------
 *
 * A "dry run" is a run in which full application functionality is not
 * required. This is usually used by unit tests targeting upper-level
 * modules that don't require the initialization of lower-level modules.
 *
 * This constructor is designed for testing purposes and exists only in debug builds.
 */
Transport::Transport(bool dryRun)
{
    controller = new Controller(dryRun);
}
#endif

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

    switch (state)
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
 * Get the controller instance
 *
 * @return pointer to controller
 */
Controller *Transport::getController() const
{
    return controller;
}

/**
 * Delete the controller we created
 */
Transport::~Transport()
{
    printf("%sTransport destructor called\n", HL_PRINT_PREFIX);

    if (controller)
    {
        delete controller;
    }
}
