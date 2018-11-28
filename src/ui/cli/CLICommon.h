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
#include <QTextStream>

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
 * Print a fixed-width QTextStream column and then return the stream to 0 width.
 */
#define QCOL(stream, width, text)   \
    stream.setFieldWidth(width);    \
    stream << text;                 \
    stream.setFieldWidth(0);

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
        // Strip one padding char for the extra space
        int colW = 32 - 1;

        QTextStream cout(stdout);
        cout.setAutoDetectUnicode(true);
        cout.setRealNumberNotation(QTextStream::FixedNotation);
        cout.setRealNumberPrecision(2);
        cout.setPadChar('.');
        cout.setFieldAlignment(QTextStream::AlignLeft);

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
            QCOL(cout, colW, CLI::tr("Device #%1").arg(i).append(" "));
            cout << " " << devices[i]->getName().c_str() << endl;

            QCOL(cout, colW, CLI::tr("Record", "device capability").append(" "));
            cout << " " << ((devices[i]->getType() & DeviceType::RECORD) ? CLI::tr("Yes") : CLI::tr("No")) << endl;

            QCOL(cout, colW, CLI::tr("Loopback", "device capability").append(" "));
            cout << " " << ((devices[i]->getType() & DeviceType::LOOPBACK) ? CLI::tr("Yes") : CLI::tr("No")) << endl;

            QCOL(cout, colW, CLI::tr("Output", "device capability").append(" "));
            cout << " " << ((devices[i]->getType() & DeviceType::PLAYBACK) ? CLI::tr("Yes") : CLI::tr("No")) << endl;

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
     * @return nullptr if not found
     */
    inline Device * findDevice(Transport *t, const std::string &name, DeviceType type)
    {
        Device *device = nullptr;
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
        if (device != nullptr)
        {
            device = new Device(*device);
        }

        Device::deleteDevices(devices);

        if (device == nullptr)
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
        int colW = 32;

        QTextStream cout(stdout);
        cout.setAutoDetectUnicode(true);
        cout.setRealNumberNotation(QTextStream::FixedNotation);
        cout.setRealNumberPrecision(2);
        cout.setFieldAlignment(QTextStream::AlignLeft);

        HulaSettings *settings = HulaSettings::getInstance();

        cout << endl;

        QCOL(cout, colW, CLI::tr("Output file:", "setting"));
        cout << QString::fromStdString(args.outputFilePath) << endl;

        // Using SI typeset for the units. Shouldn't change with localization.
        // https://english.stackexchange.com/questions/2794/punctuation-with-units
        QCOL(cout, colW, CLI::tr("Delay:"));
        cout << stod(args.delay) << " " << CLI::tr("s", "abbreviation for seconds") << endl;

        QCOL(cout, colW, CLI::tr("Duration:"));
        cout << stod(args.duration) << " " << CLI::tr("s", "abbreviation for seconds") << endl;

        QCOL(cout, colW, CLI::tr("Sample rate:"));
        cout << settings->getSampleRate() << " " << CLI::tr("Hz", "unit") << endl;

        // TODO: Change this once MP3 gets in here
        QCOL(cout, colW, CLI::tr("Encoding:"));
        cout << "WAV" << endl;

        QCOL(cout, colW, CLI::tr("Input device:"));
        cout << QString::fromStdString(args.inputDevice) << endl;

        QCOL(cout, colW, CLI::tr("Output device:"));
        cout << QString::fromStdString(args.outputDevice) << endl;
    }
}

#endif // END HULA_CLI_COMMON_H