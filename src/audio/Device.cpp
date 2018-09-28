#include "Device.h"

Device::Device(uint32_t* id, string name, DeviceType t)
{
    this->deviceID = id;
    this->deviceName = name;
    this->type = t;
    this->state = 1;
}

void Device::setID(uint32_t* id)
{
    this->deviceID = id;
}

uint32_t* Device::getID()
{
    return this->deviceID;
}

string Device::getName()
{
    return deviceName;
}

void Device::setName(string name)
{
    this->deviceName = name;
}

DeviceType Device::getType()
{
    return type;
}

void Device::setType(DeviceType t)
{
    this->type = t;
}

uint32_t Device::getType()
{
    return state;
}

void Device::setType(uint32_t s)
{
    this->state = s;
}

Device::~Device()
{

}