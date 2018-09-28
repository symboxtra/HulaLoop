#include "Controller.h"

Controller::Controller()
{
    #if defined(__unix__)
        cout << "Hello" << endl;
    #elif defined(__APPLE__)

    #elif _WIN32
        audio = new WindowsAudio();
    #endif

    if(audio == NULL){}//TODO: Handle error
}

Controller::~Controller()
{

}