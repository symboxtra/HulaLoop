#include "hlaudio/internal/Device.h"

using namespace hula;

/**
 * Constructs an instance of the Device object
 *
 * @param id ID of the audio device
 * @param name Name of the audio device
 * @param t DeviceType of the audio device
 */
Device::Device(DeviceID id, std::string name, DeviceType t)
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
DeviceID Device::getID()
{
    return deviceID;
}

/**
 * Get the name of the system audio device
 *
 * @return name String representing the name of the device
 */
std::string Device::getName()
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
 * @ingroup memory_management
 *
 * Delete all the device pointers inside the vector
 */
void Device::deleteDevices(std::vector<Device *> devices)
{
    for (auto const &device : devices)
    {
        delete device;
    }
    devices.clear();
}

/**
 * @ingroup memory_management
 *
 * Free any resources associated with the Device.
 *
 * This does not typically need to be called directly.
 * A call to Device::deleteDevices() using the vector
 * received from Controller::getDevices() is usually
 * more appropriate.
 */
Device::~Device()
{
}