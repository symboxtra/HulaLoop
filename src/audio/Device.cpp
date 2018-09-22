#include "Device.h"

Device::Device(uint32_t id, char* name)
{
    this->deviceID = id;
    this->deviceName = name;
}

void Device::setID(uint32_t id)
{
    this->deviceID = id;
}

uint32_t Device::getID()
{
    return this->deviceID;
}

char* Device::getName()
{
    return deviceName;
}

void Device::setName(char* name)
{
    this->deviceName = name;
}

Device::~Device()
{

}