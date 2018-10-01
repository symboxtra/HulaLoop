#include "Controller.h"

Controller::Controller()
{
    // Initialize OSAudio based on host OS
    #if defined(__unix__)
        //audio = new LinuxAudio();
        cout << "LINUX" << endl;
    #elif defined(__APPLE__)
        cout << "APPLE" << endl;
    #elif _WIN32
        audio = new WindowsAudio();
    #endif

    if(audio == NULL)
    {
        cout << "OS Audio error !" << endl;
    }//TODO: Handle error

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
void Controller::handleData(uint32_t size, byte* data)
{
    cout << size << endl;

    // Trigger upper layer callback functions
    for(int i = 0;i < callbackList.size();i++)
        callbackList[i]->handleData(size, data);
}

/**
 * Add upper layer functions to the callback list
 *
 * @param func Derived instance of iCallback class
 */
void Controller::addBufferReadyCallback(iCallback* func)
{
    // Add self to OSAudio callback when first callback is added
    if(this->callbackList.size() == 0)
        audio->addBufferReadyCallback(this);

    // Check if callback function already exists
    if(find(callbackList.begin(), callbackList.end(), func) == callbackList.end())
        this->callbackList.push_back(func);
}

/**
 * Remove upper layer functions to the callback list
 *
 * @param func Derived instance of iCallback class
 */
void Controller::removeBufferReadyCallback(iCallback* func)
{
    // Check if callback function exists to remove
    if(find(callbackList.begin(), callbackList.end(), func) != callbackList.end())
        this->callbackList.erase(remove(callbackList.begin(), callbackList.end(), func), callbackList.end());

    // Remove self from callback when last callback is removed
    if(this->callbackList.size() == 0)
        audio->removeBufferReadyCallback(this);
}

Controller::~Controller()
{
    delete audio;
    callbackList.clear();
}