#ifndef SYS_DEVICE
#define SYS_DEVICE

#include <cstdint>
#include <string>

using namespace std;

enum DeviceType
{
    RECORDING = 1,
    PLAYBACK = 2,
    LOOPBACK = 4
};

/**
 * Defines a system audio device
 */
// TODO: Add better public description
class Device
{
    private:
        uint32_t* deviceID;
        string deviceName;

        DeviceType type;

    public:
        Device(uint32_t* id, string name, DeviceType t);
        ~Device();

        uint32_t* getID();
        void setID(uint32_t* id);

        string getName();
        void setName(string name);

        DeviceType getType();
        void setType(DeviceType t);
};

#endif