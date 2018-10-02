#include "Controller.h"

Controller::Controller()
{
    // Initialize OSAudio based on host OS
    #if defined(__unix__)
        //audio = new LinuxAudio();
        cout << "LINUX" << endl;
    #elif defined(__APPLE__)
        audio = new OSXAudio();
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
void Controller::handleData(byte* data, uint32_t size)
{
    cout << size << endl;

    // Trigger upper layer callback functions
    for(int i = 0;i < callbackList.size();i++)
        callbackList[i]->handleData(data, size);
}

/**
 * Add upper layer functions to the callback list
 *
 * @param func Derived instance of iCallback class
 */
void Controller::addBufferReadyCallback(ICallback* func)
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
void Controller::removeBufferReadyCallback(ICallback* func)
{
    // Check if callback function exists to remove
    vector<ICallback*>::iterator it = find(callbackList.begin(), callbackList.end(), func);
    if(it != callbackList.end())
        this->callbackList.erase(it);

    // Remove self from callback when last callback is removed
    if(this->callbackList.size() == 0)
        audio->removeBufferReadyCallback(this);
}

Controller::~Controller()
{
    delete audio;
    callbackList.clear();
}