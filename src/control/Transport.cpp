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
    player = new Playback(controller);

    canRecord = true;
    canPlayback = false;
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

    canRecord = true;
    canPlayback = false;
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

    if (canRecord)
    {
        std::cout << "STARTED RECORDING" << std::endl;
        recorder->start();

        initRecordClicked = true;
        canRecord = false;
        canPlayback = false;

        state = RECORDING;
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

    if ((!canRecord && !canPlayback) || state == PAUSED)
    {
        recorder->stop();

        canRecord = false;
        canPlayback = true;

        state = STOPPED;
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

    if (canPlayback)
    {
        player->start(0);

        canPlayback = false;
        canRecord = false;

        state = PLAYING;
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

    if (state == RECORDING && !canRecord) // Pause record
    {
        recorder->stop();

        canRecord = true;
        canPlayback = true;

        state = PAUSED;
        return true;
    }
    else if (!canPlayback && initRecordClicked) // Pause playback
    {
        player->stop();

        canPlayback = true;

        state = PAUSED;
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
    canRecord = true;
    canPlayback = false;
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

    if(player)
    {
        delete player;
    }
}