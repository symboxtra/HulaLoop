#include "Controller.h"

Controller::Controller()
{

    #if defined(__unix__)
        audio = new LinuxAudio();
    #elif defined(__APPLE__)
        cout << "APPLE" << endl;
    #elif _WIN32
        audio = new WindowsAudio();
    #endif

    if(audio == NULL){}//TODO: Handle error

    audio->addBufferReadyCallback(this);
}

void Controller::handleIncomingData(uint32_t numFrames, byte* data)
{

}

void Controller::handleData(uint32_t size, byte* data)
{
    cout << "Test2" << endl;
}

void Controller::addBufferReadyCallback(f_int_t callFunction)
{
    if(find(callbackList.begin(), callbackList.end(), callFunction) == callbackList.end())
        this->callbackList.push_back(callFunction);
}

void Controller::removeBufferReadyCallback(f_int_t callFunction)
{
    if(find(callbackList.begin(), callbackList.end(), callFunction) != callbackList.end())
        this->callbackList.erase(remove(callbackList.begin(), callbackList.end(), callFunction), callbackList.end());
}

Controller::~Controller()
{
}