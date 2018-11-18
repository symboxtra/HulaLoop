#ifndef HULA_CLI_COMMON_H
#define HULA_CLI_COMMON_H

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include <QCoreApplication>

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
inline Device *findDevice(Transport *t, const std::string &name, DeviceType type)
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

    // Make a copy so that we can delete all
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
    using std::cout;
    using std::endl;
    using std::setw;
    using std::left;
    int colW = 32;

    HulaSettings *settings = HulaSettings::getInstance();

    cout << std::fixed << std::setprecision(2) << endl;

    cout << left << setw(colW) << qPrintable(CLI::tr("Output file:", "setting"));
    cout << settings->getOutputFilePath() << endl;

    // Using SI typeset for the units. Shouldn't change with localization.
    // https://english.stackexchange.com/questions/2794/punctuation-with-units
    cout << left << setw(colW) << qPrintable(CLI::tr("Delay:"));
    cout << settings->getDelayTimer() << " " << qPrintable(CLI::tr("s", "abbreviation for seconds")) << endl;

    cout << left << setw(colW) << qPrintable(CLI::tr("Duration:"));
    cout << settings->getRecordDuration() << " " << qPrintable(CLI::tr("s", "abbreviation for seconds")) << endl;

    cout << left << setw(colW) << qPrintable(CLI::tr("Sample rate:"));
    cout << settings->getSampleRate() << " " << qPrintable(CLI::tr("Hz", "unit")) << endl;

    cout << left << setw(colW) << qPrintable(CLI::tr("Encoding:"));
    cout << "WAV" << endl;

    cout << left << setw(colW) << qPrintable("Input device:");
    cout << settings->getDefaultInputDeviceName() << endl;

    cout << left << setw(colW) << qPrintable("Output device:");
    cout << settings->getDefaultOutputDeviceName() << endl;

    cout << endl;
    cout << endl;
}

#endif // END HULA_CLI_COMMON_H