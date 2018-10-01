#ifndef CONTROL
#define CONTROL

#include "OSAudio.h"
#include "iCallback.h"

#if _WIN32
    #include "WindowsAudio.h"
#elif __unix__
    #include "LinuxAudio.h"
#elif __APPLE__
    #include "OSXAudio.h"
#endif

#include <iostream>

using f_int_t = int(*)(uint32_t, byte*);
using byte = uint8_t;

class Controller : public iCallback
{
    private:
        OSAudio* audio;
        vector<iCallback*> callbackList;

    public:
        Controller();
        ~Controller();

        void handleIncomingData(uint32_t numFrames, byte* data);

        void addBufferReadyCallback(iCallback* func);
        void removeBufferReadyCallback(iCallback* func);

        void handleData(uint32_t size, byte* data);
};

#endif