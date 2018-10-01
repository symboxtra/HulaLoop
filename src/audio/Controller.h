#ifndef CONTROL
#define CONTROL

#include "OSAudio.h"
#include "iCallback.h"

#if _WIN32
    #include "WindowsAudio.h"
#elif __unix__
    // #include "LinuxAudio.h" // TODO: Remove comment once LinuxAudio is complete
#elif __APPLE__
    // #include "OSXAudio.h" // TODO: Remove comment once OSXAudio is complete
#endif

#include <iostream>

using byte = uint8_t;

// TODO: Add public description of class
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