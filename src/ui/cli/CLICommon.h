#ifndef HULA_CLI_COMMON_H
#define HULA_CLI_COMMON_H

// Convert numeric defines to strings
// https://stackoverflow.com/questions/5459868/concatenate-int-to-string-using-c-preprocessor
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

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

    /**
     * Args parsed from CLI flags.
     */
    typedef struct HulaImmediateArgs
    {
        /**
         * Signal that the caller should trigger the record immediately.
         */
        bool startRecord = false;

        /**
         * Signal that the caller should exit.
         */
        bool exit = false;

        /**
         * Store the delay as a string to be converted by
         * @ref InteractiveCLI.
         *
         * This is passed to @ref InteractiveCLI.
         */
        std::string delay = "0";

        /**
         * Store the record duration as a string to be converted
         * by @ref InteractiveCLI.
         *
         * The default of @ref HL_INFINITE_RECORD indicates
         * that the record should continue until stopped.
         *
         * This is passed to @ref InteractiveCLI.
         */
        std::string duration = STR(HL_INFINITE_RECORD);

        /**
         * Store the output file path parsed from the
         * CLI flags.
         *
         * This is passed to @ref InteractiveCLI.
         */
        std::string outputFilePath;

        /**
         * Store the input device name parsed from the
         * CLI flags.
         *
         * This is passed to @ref InteractiveCLI.
         */
        std::string inputDevice;

        /**
         * Store the output device name parsed from the
         * CLI flags.
         *
         * This is passed to @ref InteractiveCLI.
         */
        std::string outputDevice;
    } HulaImmediateArgs;

    /**
     * Utility CLI function to print the device list to the console.
     *
     * @param t Transport from which devices should be requested
     */
    inline void printDeviceList(Transport *t)
    {
        using std::cout;
        using std::endl;
        using std::setw;
        using std::left;
        int colW = 12;

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
            cout << left << setw(colW) << qPrintable(CLI::tr("Device #%1:").arg(i)) << devices[i]->getName() << endl;

            cout << left << setw(colW) << qPrintable(CLI::tr("Record:", "device capability"));
            cout << ((devices[i]->getType() & DeviceType::RECORD) ? "true" : "false") << endl;

            cout << left << setw(colW) << qPrintable(CLI::tr("Loopback:", "device capability"));
            cout << ((devices[i]->getType() & DeviceType::LOOPBACK) ? "true" : "false") << endl;

            cout << left << setw(colW) << qPrintable(CLI::tr("Output:", "device capability"));
            cout << ((devices[i]->getType() & DeviceType::PLAYBACK) ? "true" : "false") << endl;

            cout << endl;
        }

        Device::deleteDevices(devices);
    }

    /**
     * Utility function for searching the list of devices.
     *
     * Search is NOT limited to devices of DeviceType @ref type.
     * Since restricting type would mess up indexing, it is ignored
     * for now.
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

        // TODO: Change this when proper device indexing is possible
        // devices = t->getController()->getDevices(type);
        devices = t->getController()->getDevices((DeviceType)(DeviceType::LOOPBACK | DeviceType::RECORD | DeviceType::PLAYBACK));

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
            fprintf(stderr, "\n%s\n", qPrintable(CLI::tr("Could not find device matching: %1").arg(name.c_str())));
        }

        return device;
    }

    /**
     * Utility function for printing the current application settings.
     */
    inline void printSettings(const HulaImmediateArgs &args)
    {
        using std::cout;
        using std::endl;
        using std::setw;
        using std::left;
        int colW = 32;

        HulaSettings *settings = HulaSettings::getInstance();

        cout << std::fixed << std::setprecision(2) << endl;

        cout << left << setw(colW) << qPrintable(CLI::tr("Output file:", "setting"));
        cout << args.outputFilePath << endl;

        // Using SI typeset for the units. Shouldn't change with localization.
        // https://english.stackexchange.com/questions/2794/punctuation-with-units
        cout << left << setw(colW) << qPrintable(CLI::tr("Delay:"));
        cout << stod(args.delay) << " " << qPrintable(CLI::tr("s", "abbreviation for seconds")) << endl;

        cout << left << setw(colW) << qPrintable(CLI::tr("Duration:"));
        cout << stod(args.duration) << " " << qPrintable(CLI::tr("s", "abbreviation for seconds")) << endl;

        cout << left << setw(colW) << qPrintable(CLI::tr("Sample rate:"));
        cout << settings->getSampleRate() << " " << qPrintable(CLI::tr("Hz", "unit")) << endl;

        // TODO: Change this once MP3 gets in here
        cout << left << setw(colW) << qPrintable(CLI::tr("Encoding:"));
        cout << "WAV" << endl;

        cout << left << setw(colW) << qPrintable(CLI::tr("Input device:"));
        cout << args.inputDevice << endl;

        cout << left << setw(colW) << qPrintable(CLI::tr("Output device:"));
        cout << args.outputDevice << endl;

        cout << endl;
        cout << endl;
    }
}

#endif // END HULA_CLI_COMMON_H