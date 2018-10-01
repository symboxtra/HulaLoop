#include "Device.h"

Device::Device(uint32_t* id, string name, DeviceType t)
{
    this->deviceID = id;
    this->deviceName = name;
    this->type = t;
    this->state = 1;
}

/**
 * Set the ID of a system audio device
 *
 * @param id Pointer to ID value
 */
void Device::setID(uint32_t* id)
{
    this->deviceID = id;
}

/**
 * Get the ID of a system audio device
 *
 * @return id Pointer to a integer
 */
uint32_t* Device::getID()
{
    return this->deviceID;
}

/**
 * Get the name of the system audio device
 *
 * @return name String representing the name of the device
 */
string Device::getName()
{
    return deviceName;
}

/**
 * Set the name of the system audio device
 *
 * @param name String representing the name of the device
 */
void Device::setName(string name)
{
    this->deviceName = name;
}

/**
 * Get the type of system audio device
 *
 * @return type DeviceType enum value
 */
DeviceType Device::getType()
{
    return type;
}

/**
 * Set the type of system audio device
 *
 * @param type DeviceType enum value
 */
void Device::setType(DeviceType t)
{
    this->type = t;
}

/**
 * Get the state of system audio device
 *
 * @return type Integer value representing the state
 */
uint32_t Device::getState()
{
    return state;
}

/**
 * Set the state of system audio device
 *
 * @return type Integer value representing the state
 */
void Device::setState(uint32_t s)
{
    this->state = s;
}

Device::~Device()
{
    delete deviceID;

    deviceName = "";
    state = NULL;
}