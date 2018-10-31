#include <iostream>
#include <time.h>

#include "hlcontrol/internal/Export.h"
#include "hlcontrol/internal/HulaControlError.h"
#include "hlcontrol/internal/Transport.h"

/**
 * Construct a new instance of the Transport class.
 */
Transport::Transport()
{
    controller = new Controller();

    // TODO: Make the temp path dynamic so that we can congregate the files later (Only exists for demo)
    char timestamp[20];
    time_t now = time(0);
    strftime(timestamp, 20, "%Y%m%d_%H%M%S", localtime(&now));
    tempPath = Export::getTempPath() + "/hulaloop_" + std::string(timestamp);
    recorder = new Record(controller, tempPath);


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

    // TODO: Make the temp path dynamic so that we can congregate the files later
    char timestamp[20];
    time_t now = time(0);
    strftime(timestamp, 20, "%Y%m%d_%H%M%S", localtime(&now));
    tempPath = Export::getTempPath() + "/hulaloop_" + std::string(timestamp);
    recorder = new Record(controller, tempPath);

    recordState = true;
    playbackState = false;
}
#endif

/**
 * Start and handle the process of recording.
 */
bool Transport::record()
{
    std::cout << "Record button clicked!" << std::endl;
    state = RECORDING;

    if(recordState)
    {
        recorder->start();

        recordState = false;

        return true;
    }
    return false;
}

/**
 * Enter the stopped state for playback or recording.
 */
bool Transport::stop()
{
    std::cout << "Stop button clicked!" << std::endl;
    state = STOPPED;

    if(!playbackState || !recordState)
    {
        recorder->stop();

        recordState = false;
        playbackState = true;

        return true;
    }
    return false;
}

/**
 * Playback previously recorded audio.
 */
bool Transport::play()
{
    std::cout << "Play button clicked!" << std::endl;
    state = PLAYING;

    if(playbackState)
    {
        // TODO: Add playback call
        playbackState = false;

        return true;
    }
    return false;
}

/**
 * Enter the paused state for playback or recording.
 */
bool Transport::pause()
{
    std::cout << "Pause button clicked!" << std::endl;
    state = PAUSED;

    if(!recordState) // Pause record
    {
        recorder->stop();

        recordState = true;
        playbackState = true;

        return true;
    }
    else if(!playbackState) // Pause playback
    {
        // TODO: Add playback pause call
        playbackState = true;

        return true;
    }
    return false;
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

void Transport::exportFile(string targetDirectory)
{
    Export exp(targetDirectory);
    // TODO: Make it an actual directory
    exp.copyData(tempPath);
}

/**
 * Delete the controller we created
 */
Transport::~Transport()
{
    printf("%sTransport destructor called\n", HL_PRINT_PREFIX);

    if (controller)
        delete controller;

    if(recorder)
        delete recorder;
}
