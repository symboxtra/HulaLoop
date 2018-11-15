#ifndef HULA_CLI_COMMON_H
#define HULA_CLI_COMMON_H

#include <string>
#include <vector>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include <HulaVersion.h>

using namespace hula;

#define HL_CLI_NAME "hulaloop-cli"
#define HL_CLI_ASCII_HEADER "\n" \
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
 * Wrapper around translation functions for Qt.
 */
namespace hula
{
    class CLI {
        Q_DECLARE_TR_FUNCTIONS(CLI)
    };
}

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
        printf("%s: %s\n", qPrintable(CLI::tr("Device #%1").arg(i)), devices[i]->getName().c_str());
        printf("%s:   %s\n", qPrintable(CLI::tr("Record", "device capability")), (devices[i]->getType() & DeviceType::RECORD) ? "true" : "false");
        printf("%s: %s\n", qPrintable(CLI::tr("Loopback", "device capability")), (devices[i]->getType() & DeviceType::LOOPBACK) ? "true" : "false");
        printf("%s:   %s\n", qPrintable(CLI::tr("Output", "device capability")), (devices[i]->getType() & DeviceType::PLAYBACK) ? "true" : "false");
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
        (void)e;
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
        //: The argument in this text is the name or id of the device that the user searched for
        fprintf(stderr, "\n%s\n", qPrintable(CLI::tr("Could not find input device matching: %1").arg(name.c_str())));
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
    // TODO: Figure out language spacing on this
    printf("%s:\t\t\t'%s'\n", qPrintable(CLI::tr("Output file", "setting")), settings->getOutputFilePath().c_str());
    printf("%s:\t\t\t\t%.2f s\n", qPrintable(CLI::tr("Delay", "seconds")), settings->getDelayTimer());
    printf("%s:\t\t\t%.2f s\n", qPrintable(CLI::tr("Duration", "seconds")), settings->getRecordDuration());
    printf("%s:\t\t\t%d %s\n", qPrintable(CLI::tr("Sample rate")), settings->getSampleRate(), qPrintable(CLI::tr("Hz", "unit")));
    printf("%s:\t\t\t%s\n", qPrintable(CLI::tr("Encoding")), "WAV");
    printf("%s:\t\t\t%s\n", qPrintable("Input device"), settings->getDefaultInputDeviceName().c_str());
    printf("%s:\t\t\t%s\n", qPrintable("Output device"), settings->getDefaultOutputDeviceName().c_str());
    printf("\n");
}

#endif // END HULA_CLI_COMMON_H