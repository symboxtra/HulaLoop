#include "Device.h"

Device::Device(uint32_t id)
{
    this->deviceID = id;
}

void Device::setID(uint32_t id)
{
    this->deviceID = id;
}

uint32_t Device::getID()
{
    return this->deviceID;
}

Device::~Device()
{

}