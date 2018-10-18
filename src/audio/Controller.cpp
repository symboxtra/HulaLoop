#include <algorithm>
#include <fstream>

#include "hlaudio/internal/Controller.h"

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

    // Add current Controller instance as buffercallback to
    // OSAudio
    audio->addBufferReadyCallback(this);
}

/**
 * Callback function that is triggered when audio is captured
 * by OSAudio
 *
 * @param size Size of returned audio data (frames)
 * @param data Audio data in byte buffer
 */
void Controller::handleData(uint8_t *data, uint32_t size)
{
    cout << "Data received: " << size << endl;

    // Trigger upper layer callback functions
    for (int i = 0; i < callbackList.size(); i++)
    {
        callbackList[i]->handleData(data, size);
    }
}

/**
 * Add upper layer functions to the callback list
 *
 * @param func Derived instance of iCallback class
 */
void Controller::addBufferReadyCallback(ICallback *func)
{
    // Add self to OSAudio callback when first callback is added
    if (this->callbackList.size() == 0)
    {
        audio->addBufferReadyCallback(this);
    }

    // Check if callback function already exists
    if (find(callbackList.begin(), callbackList.end(), func) == callbackList.end())
    {
        this->callbackList.push_back(func);
    }
}

/**
 * Remove upper layer functions to the callback list
 *
 * @param func Derived instance of iCallback class
 */
void Controller::removeBufferReadyCallback(ICallback *func)
{
    // Check if callback function exists to remove
    vector<ICallback *>::iterator it = find(callbackList.begin(), callbackList.end(), func);
    if (it != callbackList.end())
    {
        this->callbackList.erase(it);
    }

    // Remove self from callback when last callback is removed
    if (this->callbackList.size() == 0)
    {
        audio->removeBufferReadyCallback(this);
    }
}

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
HulaRingBuffer * Controller::createBuffer(float duration)
{
    return new HulaRingBuffer(duration);
}

/**
 * Allocate and initialize a HulaRingBuffer and automatically
 * add it to the OSAudio ring buffer list.
 */
HulaRingBuffer * Controller::createAndAddBuffer(float duration)
{
    HulaRingBuffer * rb = new HulaRingBuffer(duration);
    addBuffer(rb);
    return rb;
}

/**
 * Get input devices from OSAudio.
 */
vector<Device *> Controller::getInputDevices()
{
    return audio->getInputDevices();
}

/**
 * Get output devices from OSAudio.
 */
vector<Device *> Controller::getOutputDevices()
{
    return audio->getOutputDevices();
}

/**
 * Deconstructs the current instance of the Controller class.
 */
Controller::~Controller()
{
    // Don't do this until mem management is fixed
    // delete audio;
    callbackList.clear();
}
