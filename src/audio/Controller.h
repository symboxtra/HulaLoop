#ifndef CONTROL
#define CONTROL

#include "OSAudio.h"
#if _WIN32
    #include "WindowsAudio.h"
#elif __unix__
    #include "LinuxAudio.h"
#elif __APPLE__
    #include "OSXAudio.h"
#endif

#include <iostream>

using f_int_t = int(*)(uint32_t, byte*);

class Controller
{
    private:
        OSAudio* audio;
        vector<f_int_t> callbackList;

    public:
        Controller();
        ~Controller();

        void addBufferReadyCallback(f_int_t func);
        void removeBufferReadyCallback(f_int_t callFunction);
};

#endif