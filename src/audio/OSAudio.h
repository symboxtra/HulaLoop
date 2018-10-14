#ifndef OS_AUDIO
#define OS_AUDIO

// System
#include <stdlib.h>
#include <thread>
#include <vector>

#include "Device.h"
#include "ICallback.h"

using namespace std;

// TODO: Add better public description
/**
 * An abstract class that defines the components of the particular OS
 * specfic classes.
 */
class OSAudio
{
    protected:

        /**
         * The selected input device
         */
        Device* activeInputDevice;

        /**
         * The selected output device
         */
        Device* activeOutputDevice;

        /**
         * List of all added callback function
         */
        vector<ICallback*> callbackList;

        /**
         * List of all running threads
         */
        vector<thread> execThreads;

        uint32_t captureBufferSize;

    public:
        virtual ~OSAudio() = 0;

        void setBufferSize(uint32_t size);

        void addBufferReadyCallback(ICallback* c);
        void removeBufferReadyCallback(ICallback* func);

        /**
         * Receive the list of available output audio devices connected to the OS
         * and return them as Device instances
         *
         * @return vector of Device instances
         */
        virtual vector<Device*> getInputDevices() = 0;

        /**
         * Set the selected output device and restart capture threads with
         * new device
         *
         * @param device Instance of Device that corresponds to the desired system device
         */
        virtual vector<Device*> getOutputDevices() = 0;

        /**
         * Execution loop for loopback capture
         */
        virtual void capture() = 0;

        void setActiveRecordDevice(Device* device); //TODO: Make virtual or maybe remove if can combine into one function
        virtual void setActiveOutputDevice(Device* device) = 0;
};

#endif