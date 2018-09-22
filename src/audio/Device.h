#ifndef SYS_DEVICE
#define SYS_DEVICE

#include <cstdint>
//#include <string>

class Device
{
    private:
        uint32_t deviceID;
        //string deviceName;

    public:
        Device(uint32_t id);
        ~Device();

        uint32_t getID();
        void setID(uint32_t id);

        //string getDeviceName();
        //void setDeviceName(string name);
};

#endif