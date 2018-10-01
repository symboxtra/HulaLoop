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

void Controller::handleData(uint32_t size, byte* data)
{
    cout << size << endl;
}

void Controller::addBufferReadyCallback(iCallback* func)
{
    if(find(callbackList.begin(), callbackList.end(), func) == callbackList.end())
        this->callbackList.push_back(func);
}

void Controller::removeBufferReadyCallback(iCallback* func)
{
    if(find(callbackList.begin(), callbackList.end(), func) != callbackList.end())
        this->callbackList.erase(remove(callbackList.begin(), callbackList.end(), func), callbackList.end());
}

Controller::~Controller()
{
}