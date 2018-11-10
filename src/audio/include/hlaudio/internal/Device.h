#ifndef HL_DEVICE_H
#define HL_DEVICE_H

#include <cstdint>
#include <string>
#include <vector>

using namespace std;

enum DeviceType
{
    RECORD = 1,
    PLAYBACK = 2,
    LOOPBACK = 4
};

union DeviceID
{
    string linuxID;
    uint32_t *windowsID;
};

/**
 * Defines a system audio device
 */
// TODO: Add better public description
class Device {
    private:
        uint32_t *deviceID;
        string deviceName;

        DeviceType type;

    public:
        Device(uint32_t *id, string name, DeviceType t);
        ~Device();

        uint32_t *getID();

        string getName();

        DeviceType getType();

        static void deleteDevices(vector<Device *> devices);
};

#endif // END HL_DEVICE_H