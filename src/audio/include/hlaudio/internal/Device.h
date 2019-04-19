#ifndef HL_DEVICE_H
#define HL_DEVICE_H

#include <cstdint>
#include <string>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace hula
{
    /**
     * Denotes type of Device.
     * Meant to be bitwise or'd together to support multiple capabilities.
     */
    enum DeviceType
    {
        RECORD = 1,
        LOOPBACK = 2,
        PLAYBACK = 4,
        INPUT = 3,  // RECORD | LOOPBACK
        OUTPUT = 4, // PLAYBACK
        ANY = 7     // RECORD | LOOPBACK | PLAYBACK
    };

    /**
     * Struct for the three types of device ID.
     *
     * PortAudio devices should use @ref portAudioID.
     * Linux ALSA devices should use @ref linuxID.
     * Windows WASAPI devices should use @ref windowsID.
     */
    struct DeviceID
    {
        /**
         * ID used by ALSA devices.
         */
        std::string linuxID = "";

        /**
         * ID used by PortAudio devices.
         */
        int portAudioID = -1;

        #ifdef _WIN32
        /**
         * ID used by Windows WASAPI devices.
         */
        LPWSTR windowsID = nullptr;
        #endif
    };

    /**
     * Wrapper for OS specific device information.
     */
    class Device {
        private:
            DeviceID deviceID;
            std::string deviceName;

            DeviceType type;

        public:
            Device(DeviceID id, std::string name, DeviceType t);
            ~Device();

            DeviceID getID();

            std::string getName();

            DeviceType getType();

            static void deleteDevices(std::vector<Device *> devices);
    };
}

#endif // END HL_DEVICE_H