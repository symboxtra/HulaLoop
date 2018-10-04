#ifndef CONTROL
#define CONTROL

#include "OSAudio.h"
#include "ICallback.h"

#if _WIN32
    #include "WindowsAudio.h"
#elif __unix__
    // #include "LinuxAudio.h" // TODO: Remove comment once LinuxAudio is complete
#elif __APPLE__
    #include "OSXAudio.h"
#endif

#include <iostream>

using byte = uint8_t;

// TODO: Add public description of class
/**
 * A class that structures the receival of audio from the OS framework
 */
class Controller : public ICallback
{
    private:
        OSAudio* audio;

        vector<ICallback*> callbackList;

    public:
        Controller();
        ~Controller();

        void addBufferReadyCallback(ICallback* func);
        void removeBufferReadyCallback(ICallback* func);

        void handleData(byte* data, uint32_t size);
};

#endif