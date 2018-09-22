#include "Device.h"

Device::Device(LPCWSTR id, string name, DeviceType t)
{
    this->deviceID = id;
    this->deviceName = name;
    this->type = t;
}

void Device::setID(LPCWSTR id)
{
    this->deviceID = id;
}

LPCWSTR Device::getID()
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

Device::~Device()
{

}