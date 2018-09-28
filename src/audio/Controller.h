#ifndef CONTROL
#define CONTROL

#include "OSAudio.h"
#if _WIN32
    #include "WindowsAudio.h"
#endif

#include <iostream>

class Controller
{
    private:
        OSAudio* audio;
    public:
        Controller();
        ~Controller();
};

#endif