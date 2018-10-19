#ifndef OS_AUDIO
#define OS_AUDIO

// System
#include <cstdlib>
#include <thread>
#include <vector>

#include "Device.h"
#include "HulaRingBuffer.h"
#include "ICallback.h"

using namespace std;

// TODO: Add better public description
/**
 * An abstract class that defines the components of the particular OS
 * specfic classes.
 */
class OSAudio {
    protected:

        /**
         * The selected input device
         */
        Device *activeInputDevice;

        /**
         * The selected output device
         */
        Device *activeOutputDevice;

        /**
         * List of all added callback function
         */
        vector<ICallback *> callbackList;

        /**
         * List of all added ring buffers.
         * Data received from the operating system is copied into each of these buffers.
         */
        vector<HulaRingBuffer *> rbs;

        /**
         * List of all running threads.
         *
         * TODO: Remove
         * Leave until WindowsAudio can be updated.
         */
        vector<thread> execThreads;

        /**
         * Thread for input device activities.
         */
        vector<thread> inThreads;

        /**
         * Thread for output device activities.
         */
        vector<thread> outThreads;

        /**
         * Flag to syncronize the capture thread for an instance.
         * This is used to break the capture loop when switching devices.
         */
        volatile bool deviceSwitch;

        /**
         * Flag representing capture state.
         * True if capturing data.
         * False if idle (no ring buffers present or device is switching).
         *
         * Mostly for testing.
         */
        volatile bool isCapturing;

        uint32_t captureBufferSize;

    public:
        virtual ~OSAudio() = 0;

        void setBufferSize(uint32_t size);

        void addBufferReadyCallback(ICallback *c);
        void removeBufferReadyCallback(ICallback *func);

        void addBuffer(HulaRingBuffer *rb);
        void removeBuffer(HulaRingBuffer *rb);
        void copyToBuffers(const void *data, uint32_t bytes);

        static void backgroundCapture(OSAudio *_this);

        /**
         * Receive the list of available output audio devices connected to the OS
         * and return them as Device instances
         *
         * @return vector of Device instances
         */
        virtual vector<Device *> getInputDevices() = 0;

        /**
         * Set the selected output device and restart capture threads with
         * new device
         *
         * @param device Instance of Device that corresponds to the desired system device
         */
        virtual vector<Device *> getOutputDevices() = 0;

        /**
         * Execution loop for loopback capture
         */
        virtual void capture() = 0;

        void setActiveRecordDevice(Device *device); // TODO: Make virtual or maybe remove if can combine into one function
        virtual void setActiveOutputDevice(Device *device) = 0;
};

#endif