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
        LPCWSTR deviceID;
        string deviceName;

        DeviceType type;

    public:
        Device(LPCWSTR id, string name, DeviceType t);
        ~Device();

        LPCWSTR getID();
        void setID(LPCWSTR id);

        string getName();
        void setName(string name);

        DeviceType getType();
        void setType(DeviceType t);
};

#endif