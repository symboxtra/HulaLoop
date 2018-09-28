#ifndef SYS_DEVICE
#define SYS_DEVICE

#include <cstdint>
#include <string>

#include <windows.h>

using namespace std;

enum class DeviceType { RECORDING, PLAYBACK };

class Device
{
    private:
        uint32_t* deviceID;
        string deviceName;

        DeviceType type;
        uint32_t state;

    public:
        Device(uint32_t* id, string name, DeviceType t);
        ~Device();

        uint32_t* getID();
        void setID(uint32_t* id);

        string getName();
        void setName(string name);

        DeviceType getType();
        void setType(DeviceType t);

        uint32_t getState();
        void setState(); 
};

#endif