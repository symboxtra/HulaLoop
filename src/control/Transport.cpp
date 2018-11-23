#include <iostream>
#include <time.h>

#include "hlcontrol/internal/Export.h"
#include "hlcontrol/internal/HulaControlError.h"
#include "hlcontrol/internal/Transport.h"

using namespace hula;

/**
 * Construct a new instance of the Transport class.
 */
Transport::Transport()
{
    controller = new Controller();
    recorder = new Record(controller);

    recordState = true;
    playbackState = false;
    initRecordClicked = false;
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
    recorder = new Record(controller);

    recordState = true;
    playbackState = false;
}
#endif

/**
 * Start and handle the process of recording.
 *
 * @param delay Time, in seconds, to wait before starting record
 * @param duration Time, in seconds, to record for.
 *
 * @return Successful start of recording
 */
bool Transport::record(double delay, double duration)
{
    std::cout << "Record triggered!" << std::endl;
    std::cout << "Delay set to: " << delay << std::endl;
    std::cout << "Duration set to: " << duration << std::endl;
    state = RECORDING;

    if (recordState)
    {
        std::cout << "STARTED RECORDING" << std::endl;
        recorder->start();

        initRecordClicked = true;
        recordState = false;

        return true;
    }
    return false;
}

/**
 * Overload of record with no delay and infinite
 * record time.
 *
 * @return Successful start of recording
 */
bool Transport::record()
{
    return record(0, HL_INFINITE_RECORD);
}

/**
 * Enter the stopped state for playback or recording.
 */
bool Transport::stop()
{
    std::cout << "Stop button clicked!" << std::endl;
    state = STOPPED;

    if (!recordState || playbackState)
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

    if (playbackState)
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

    if (!recordState) // Pause record
    {
        recorder->stop();

        recordState = true;
        playbackState = true;

        return true;
    }
    // TODO: Currently a bool is used to check if record is successfully clicked atleast once
    // TODO: Find a better way to do it?
    else if (!playbackState && initRecordClicked) // Pause playback
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

void Transport::exportFile(std::string targetDirectory)
{
    Export exp(targetDirectory);
    exp.copyData(recorder->getExportPaths());

    recorder->clearExportPaths();
}

/**
 * Reset transport states and delete captured audio files from system temp folder
 */
void Transport::deleteTempFiles()
{
    // Reset states
    recordState = true;
    playbackState = false;
    state = (TransportState)-1;

    // Delete audio files from system temp folder
    Export::deleteTempFiles(recorder->getExportPaths());
    recorder->clearExportPaths();
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

    if (recorder)
    {
        delete recorder;
    }
}