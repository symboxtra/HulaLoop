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

    if (!playbackState || !recordState)
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
    else if (!playbackState) // Pause playback
    {
        // TODO: Add playback pause call
        playbackState = true;

        return true;
    }
    return false;
}

/**
 * Discard any recorded audio and reset the state.
 *
 * @return bool Success of command
 */
/*
bool Transport::Discard()
{
    state = INITIAL;
    recordState = true;
    playbackState = false;

    return true;
}
*/

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
    // TODO: Remove harcoded path (Only for demo)
    #if WIN32
    vector<std::string> temp;
    temp.push_back("C:\\Users\\patel\\AppData\\Local\\Temp\\hulaloop_temp.wav");
    exp.copyData(temp);
    #else
    exp.copyData(recorder->getExportPaths());
    recorder->clearExportPaths();
    #endif
}

/**
 * Send the files from record to export for deletion
 */
void Transport::deleteTempFiles()
{
    recordState = true;
    playbackState = false;
    Export::deleteTempFiles(recorder->getExportPaths());
    recorder->clearExportPaths();
}

/**
 * Checks if there are files in the export paths which means
 * recording has happened and there are no files left to export
 *
 * @return true is the user has recorded files
 */
bool Transport::hasExportPaths()
{
    bool exportPaths = recorder->getExportPaths().empty();
    if (exportPaths)
    {
        // there are no paths left
        return false;
    }
    else
    {
        // there are paths left
        return true;
    }
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
