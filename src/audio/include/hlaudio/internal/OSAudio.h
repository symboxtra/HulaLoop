#ifndef OS_AUDIO
#define OS_AUDIO

// System
#include <atomic>
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
    private:
        void joinAndKillThreads(vector<thread>& threads);

    protected:

        /**
         * Constructor is protected since this class is abstract.
         */
        OSAudio() {};

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
         * This is used to break the capture loop when switching devices
         * or when 0 buffers are present.
         *
         * Should never be set directly. Only by setActiveXXXDevice().
         */
        atomic<bool> endCapture;

        uint32_t captureBufferSize;

    public:
        virtual ~OSAudio() = 0;

        void setBufferSize(uint32_t size);

        void addBufferReadyCallback(ICallback *c);
        void removeBufferReadyCallback(ICallback *func);

        void addBuffer(HulaRingBuffer *rb);
        void removeBuffer(HulaRingBuffer *rb);
        void copyToBuffers(const void *data, uint32_t bytes);

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
        static void backgroundCapture(OSAudio *_this);
        virtual bool checkRates(Device *device) = 0;

        void setActiveInputDevice(Device *device);
        void setActiveOutputDevice(Device *device);
};

#endif