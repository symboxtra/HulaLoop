#ifndef SYS_DEVICE
#define SYS_DEVICE

#include <cstdint>

class Device
{
    private:
        uint32_t deviceID;

    public:
        Device(uint32_t id);
        ~Device();

        uint32_t getID();
        void setID(uint32_t id);
};

#endif