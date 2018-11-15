#ifndef HL_DEVICE_H
#define HL_DEVICE_H

#include <cstdint>
#include <string>
#include <vector>

namespace hula
{
    /**
     * Denotes type of Device.
     * Meant to be bitwise or'd together to support multiple capabilities.
     */
    enum DeviceType
    {
        RECORD = 1,
        PLAYBACK = 2,
        LOOPBACK = 4
    };

    /**
     * Union for the three types of device ID.
     */
    union DeviceID
    {
        /**
         * Device ID used by @ref LinuxAudio.
         */
        std::string linuxID;

        /**
         * Device ID used by @ref WindowsAudio.
         */
        uint32_t *windowsID;

        /**
         * Device ID used by @ref OSXAudio.
         */
        int osxID;
    };

    /**
     * Wrapper for OS specific device information.
     */
    class Device {
        private:
            uint32_t *deviceID;
            std::string deviceName;

            DeviceType type;

        public:
            Device(uint32_t *id, std::string name, DeviceType t);
            ~Device();

            uint32_t *getID();

            std::string getName();

            DeviceType getType();

            static void deleteDevices(std::vector<Device *> devices);
    };
}

#endif // END HL_DEVICE_H