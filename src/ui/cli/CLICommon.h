#ifndef HULA_CLI_COMMON_H
#define HULA_CLI_COMMON_H

#include <string>
#include <vector>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include <HulaVersion.h>

#define HL_ASCII_HEADER "\n" \
    "   _    _       _       _\n" \
    "  | |  | |     | |     | |\n" \
    "  | |__| |_   _| | __ _| |     ___   ___  _ __\n" \
    "  |  __  | | | | |/ _` | |    / _ \\ / _ \\| '_ \\\n" \
    "  | |  | | |_| | | (_| | |___| (_) | (_) | |_) |\n" \
    "  |_|  |_|\\__,_|_|\\__,_|______\\___/ \\___/| .__/\n" \
    "                                         | |\n" \
    "                                         |_|\n" \
    "----------------------------------------------------\n\n" \

/**
 * Utility CLI function to print the device list to the console.
 *
 * @param t Transport from which devices should be requested
 */
inline void printDeviceList(Transport *t)
{
    HulaSettings *settings = HulaSettings::getInstance();

    std::vector<Device *> devices;
    if (settings->getShowRecordDevices())
    {
        devices = t->getController()->getDevices((DeviceType)(PLAYBACK | LOOPBACK | RECORD));
    }
    else
    {
        devices = t->getController()->getDevices((DeviceType)(PLAYBACK | LOOPBACK));
    }

    printf("\n");
    for (size_t i = 0; i < devices.size(); i++)
    {
        printf("Device #%lu: %s\n", i, devices[i]->getName().c_str());
        printf("Record:   %s\n", (devices[i]->getType() & DeviceType::RECORD) ? "true" : "false");
        printf("Loopback: %s\n", (devices[i]->getType() & DeviceType::LOOPBACK) ? "true" : "false");
        printf("Output:   %s\n", (devices[i]->getType() & DeviceType::PLAYBACK) ? "true" : "false");
        printf("\n");
    }

    Device::deleteDevices(devices);
}

/**
 * Utility function for searching the list of devices.
 *
 * Search is limited to devices of DeviceType @ref type.
 *
 * @param t Transport from which devices should be requested
 * @param name Name of the desired device
 * @param type Expected type of the desired device
 * @return Pointer to newly allocated Device object if found
 * @return NULL if not found
 */
inline Device * findDevice(Transport *t, const std::string &name, DeviceType type)
{
    Device *device = NULL;
    std::vector<Device *> devices;
    devices = t->getController()->getDevices(type);

    // Check if we got a numeric id
    // Since we all do this differently,
    // we'll just use the relative ordering to pick
    // ids
    int id = -1;
    try
    {
        id = std::stoi(name, nullptr);
    }
    catch (std::invalid_argument &e)
    {
        // Wasn't an id
    }

    for (size_t i = 0; i < devices.size(); i++)
    {
        // Check id and name
        if (i == id || devices[i]->getName() == name)
        {
            device = devices[i];
            break;
        }
    }

    // Make a copy so that we can delete
    if (device != NULL)
    {
        device = new Device(*device);
    }

    Device::deleteDevices(devices);

    if (device == NULL)
    {
        fprintf(stderr, "\nCould not find input device matching: %s\n", name.c_str());
    }

    return device;
}

/**
 * Utility function for printing the current application settings.
 */
inline void printSettings()
{
    HulaSettings *settings = HulaSettings::getInstance();

    printf("\n");
    printf("Output file:          '%s'\n", settings->getOutputFilePath().c_str());
    printf("Delay:                %.2f s\n", settings->getDelayTimer());
    printf("Length:               %.2f s\n", settings->getRecordDuration());
    printf("Sample rate:          %'d Hz\n", settings->getSampleRate());
    printf("Encoding:             %s\n", "WAV");
    printf("Input device:         '%s'\n", settings->getDefaultInputDeviceName().c_str());
    printf("Output device:        '%s'\n", settings->getDefaultOutputDeviceName().c_str());
    printf("\n");
}

#endif // END HULA_CLI_COMMON_H