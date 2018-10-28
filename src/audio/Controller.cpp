#include <algorithm>
#include <fstream>

#include "hlaudio/internal/Controller.h"
#include "include/hlaudio/internal/HulaAudioError.h"

#if _WIN32
    #include "WindowsAudio.h"
#elif __unix__
    #include "LinuxAudio.h"
#elif __APPLE__
    #include "OSXAudio.h"
#endif

/**
 * Construct an instance of Controller class.
 * Acts as a bridge between the higher levels and OS level functions
 */
Controller::Controller()
{
    // Initialize OSAudio based on host OS
    #if defined(__unix__)
    audio = new LinuxAudio();
    #elif defined(__APPLE__)
    audio = new OSXAudio();
    #elif _WIN32
    audio = new WindowsAudio();
    #endif

    if (audio == NULL)
    {
        cerr << "OS Audio error !" << endl;
    } // TODO: Handle error

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
Controller::Controller(bool dryRun)
{
    audio = NULL;
}
#endif

/**
 * Add an initialized buffer to the list of buffers that receive audio data.
 * As soon as the buffer is added, it should begin receiving data.
 *
 * If already present, the ring buffer will not be duplicated.
 *
 * This is a publicly exposed wrapper for the OSAudio method.
 *
 * @param rb HulaLoop ring buffer to add to the list.
*/
void Controller::addBuffer(HulaRingBuffer *rb)
{
    audio->addBuffer(rb);
}

/**
 * Remove a buffer from the list of buffers that receive audio data.
 * The removed buffer is not deleted and must be deleted by the user.
 *
 * This enables pausing retrieval when audio is not needed and
 * re-adding the same buffer when audio data is again needed.
 *
 * This is a publicly exposed wrapper for the OSAudio method.
 *
 * @param rb HulaLoop ring buffer to remove from the list.
*/
void Controller::removeBuffer(HulaRingBuffer *rb)
{
    audio->removeBuffer(rb);
}

/**
 * Allocate and initialize a HulaRingBuffer that can be added to
 * the OSAudio ring buffer list via Controller::addBuffer.
 *
 * @return Newly allocated ring buffer.
 */
HulaRingBuffer *Controller::createBuffer(float duration)
{
    return new HulaRingBuffer(duration);
}

/**
 * Allocate and initialize a HulaRingBuffer and automatically
 * add it to the OSAudio ring buffer list.
 */
HulaRingBuffer *Controller::createAndAddBuffer(float duration)
{
    HulaRingBuffer *rb = new HulaRingBuffer(duration);
    addBuffer(rb);
    return rb;
}

vector<Device*> Controller::getDevices(DeviceType type) const
{
    return audio->getDevices(type);
}

/**
 * Middle function to transfer the device from the front end
 */
void Controller::setActiveInputDevice(Device *device) const
{
    audio->setActiveInputDevice(device);
}

void Controller::setActiveOutputDevice(Device *device) const
{
    audio->setActiveOutputDevice(device);
}

/**
 * Deconstructs the current instance of the Controller class
 */
Controller::~Controller()
{
    printf("%sController destructor called\n", HL_PRINT_PREFIX);

    // Don't do this until mem management is fixed
    if (audio)
    {
        delete audio;
    }
}
