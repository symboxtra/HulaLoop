#include "hlaudio/internal/Device.h"

/**
 * Constructs an instance of the Device object
 *
 * @param id ID of the audio device
 * @param name Name of the audio device
 * @param t DeviceType of the audio device
 */
Device::Device(uint32_t *id, string name, DeviceType t)
{
    this->deviceID = id;
    this->deviceName = name;
    this->type = t;
}

/**
 * Get the ID of a system audio device
 *
 * @return id Pointer to a integer
 */
uint32_t *Device::getID()
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
 * Get the type of system audio device
 *
 * @return type DeviceType enum value
 */
DeviceType Device::getType()
{
    return type;
}

/**
 * Delete all the device pointers inside the vector
 */
void Device::deleteDevices(vector<Device *> devices)
{
    for (auto const &device : devices)
    {
        // cout << device->getName() << endl;
        delete device;
    }
    devices.clear();
}

/**
 * Deconstructs the device instance
 */
Device::~Device()
{
    /*if (deviceID)
    {
        delete deviceID;
    }*/

    deviceName.clear();
}