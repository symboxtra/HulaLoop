#ifndef SYS_DEVICE
#define SYS_DEVICE

#include <cstdint>
//#include <string>

class Device
{
    private:
        uint32_t deviceID;
        char* deviceName;

    public:
        Device(uint32_t id, char* name);
        ~Device();

        uint32_t getID();
        void setID(uint32_t id);

        char* getName();
        void setName(char* name);
};

#endif