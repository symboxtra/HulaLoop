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
    hlDebug() << "Record triggered!" << std::endl;
    hlDebug() << "Delay set to: " << delay << std::endl;
    hlDebug() << "Duration set to: " << duration << std::endl;
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
    hlDebug() << "Stop button clicked!" << std::endl;
    state = STOPPED;

    if(!recordState)
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
    hlDebug() << "Play button clicked!" << std::endl;
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
    hlDebug() << "Pause button clicked!" << std::endl;
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
            return qPrintable(tr("Recording"));
        case STOPPED:
            return qPrintable(tr("Stopped"));
        case PLAYING:
            return qPrintable(tr("Playing"));
        case PAUSED:
            return qPrintable(tr("Paused"));
        default:
            return qPrintable(tr("Unknown"));

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
    // TODO: Make it an actual directory
    exp.copyData("/tmp/temp.txt");
}

/**
 * Delete the controller we created
 */
Transport::~Transport()
{
    hlDebugf("Transport destructor called\n");

    if (controller)
    {
        delete controller;
    }
}
