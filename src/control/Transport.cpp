#include <chrono>

#include "hlcontrol/internal/Export.h"
#include "hlcontrol/internal/HulaControlError.h"
#include "hlcontrol/internal/HulaSettings.h"
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
    state = READY;
}

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

    if (recordState)
    {
        hlDebug() << "STARTED RECORDING" << std::endl;
        recorder->start();

        initRecordClicked = true;
        recordState = false;

        std::this_thread::sleep_for(std::chrono::milliseconds(HL_TRANSPORT_LOCKOUT_MS));
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

    if (!recordState || playbackState)
    {
        recorder->stop();

        recordState = false;
        playbackState = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(HL_TRANSPORT_LOCKOUT_MS));
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

    if (playbackState)
    {
        // TODO: Add playback call
        playbackState = false;

        std::this_thread::sleep_for(std::chrono::milliseconds(HL_TRANSPORT_LOCKOUT_MS));
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

    if (!recordState) // Pause record
    {
        recorder->stop();

        recordState = true;
        playbackState = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(HL_TRANSPORT_LOCKOUT_MS));
        return true;
    }
    // TODO: Currently a bool is used to check if record is successfully clicked atleast once
    // TODO: Find a better way to do it?
    else if (!playbackState && initRecordClicked) // Pause playback
    {
        // TODO: Add playback pause call
        playbackState = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(HL_TRANSPORT_LOCKOUT_MS));
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
    std::cerr << state << std::endl;
    switch (state)
    {
        case READY:
            return std::string(qPrintable(tr("Ready", "state")));
        case RECORDING:
            return std::string(qPrintable(tr("Recording", "state")));
        case STOPPED:
            return std::string(qPrintable(tr("Stopped", "state")));
        case PLAYING:
            return std::string(qPrintable(tr("Playing", "state")));
        case PAUSED:
            return std::string(qPrintable(tr("Paused", "state")));
        default:
            return std::string(qPrintable(tr("Unknown", "state")));

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
void Transport::discard()
{
    // Reset states
    recordState = true;
    playbackState = false;
    state = READY;

    // Delete audio files from system temp folder
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
    hlDebugf("Transport destructor called\n");

    if (recorder)
    {
        delete recorder;
    }

    if (controller)
    {
        delete controller;
    }
}