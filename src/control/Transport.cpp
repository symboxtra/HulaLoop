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

    canRecord = true;
    canPlayback = false;
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

    if (canRecord)
    {
        hlDebug() << "STARTED RECORDING" << std::endl;
        recorder->start();

        initRecordClicked = true;
        canRecord = false;
        canPlayback = false;

        state = RECORDING;
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

    if ((!canRecord && !canPlayback) || state == PAUSED)
    {
        recorder->stop();

        canRecord = false;
        canPlayback = true;

        state = STOPPED;
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

    if (canPlayback)
    {
        // TODO: Add start playback call

        canPlayback = false;
        canRecord = false;

        state = PLAYING;
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

    if (state == RECORDING && !canRecord) // Pause record
    {
        recorder->stop();

        canRecord = true;
        canPlayback = true;

        state = PAUSED;
        std::this_thread::sleep_for(std::chrono::milliseconds(HL_TRANSPORT_LOCKOUT_MS));
        return true;
    }
    else if (!canPlayback && initRecordClicked) // Pause playback
    {
        // TODO: Add playback pause call

        canPlayback = true;

        state = PAUSED;
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
    canRecord = true;
    canPlayback = false;
    initRecordClicked = false;
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