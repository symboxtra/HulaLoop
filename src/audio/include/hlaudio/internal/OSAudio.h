#ifndef OS_AUDIO
#define OS_AUDIO

// System
#include <atomic>
#include <cstdlib>
#include <thread>
#include <vector>

#include "Device.h"
#include "HulaRingBuffer.h"

using namespace std;

// TODO: Add better public description
/**
 * An abstract class that defines the components of the particular OS
 * specfic classes.
 */
class OSAudio {
    private:
        void joinAndKillThreads(vector<thread> &threads);

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
         * List of all added ring buffers.
         * Data received from the operating system is copied into each of these buffers.
         */
        vector<HulaRingBuffer *> rbs;

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

        /**
         * Desired capture buffer size
         */
        uint32_t captureBufferSize;

    public:
        virtual ~OSAudio() = 0;

        void setBufferSize(uint32_t size);

        void addBuffer(HulaRingBuffer *rb);
        void removeBuffer(HulaRingBuffer *rb);
        void copyToBuffers(const void *data, uint32_t bytes);

        /**
         * Receive the list of available record, playback and/or loopback audio devices
         * connected to the OS and return them as Device instances
         *
         * @param type DeviceType that is combination from the DeviceType enum
         * @return vector<Device*> A list of Device instances that carry the necessary device information
         */
        virtual vector<Device *> getDevices(DeviceType type) = 0;

        /**
         * Execution loop for loopback capture
         */
        virtual void capture() = 0;
        static void backgroundCapture(OSAudio *_this);

        /**
         * Verify the bit rate of set rate with the hardware device compatibility
         */
        virtual bool checkRates(Device *device) = 0;

        void setActiveInputDevice(Device *device);
        void setActiveOutputDevice(Device *device);
};

#endif