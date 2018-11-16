#ifndef HL_CLI_ARGS_H
#define HL_CLI_ARGS_H

#include <cstdio>
#include <QCommandLineParser>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include "CLICommon.h"

using namespace hula;

/**
 * Define the short opt and long opt strings
 * These are used in multiple places, so defining
 * will make them easier to replace
 */
#define HL_OUT_FILE_SO        "f"
#define HL_OUT_FILE_LO        "out-file"
#define HL_DELAY_TIME_SO      "d"
#define HL_DELAY_TIME_LO      "delay"
#define HL_RECORD_TIME_SO     "t"
#define HL_RECORD_TIME_LO     "time"
#define HL_TRIGGER_RECORD_SO  "r"
#define HL_TRIGGER_RECORD_LO  "record"
#define HL_SAMPLE_RATE_SO     "s"
#define HL_SAMPLE_RATE_LO     "sample-rate"
#define HL_ENCODING_SO        "e"
#define HL_ENCODING_LO        "encoding"
#define HL_INPUT_DEVICE_SO    "i"
#define HL_INPUT_DEVICE_LO    "input-device"
#define HL_OUTPUT_DEVICE_SO   "o"
#define HL_OUTPUT_DEVICE_LO   "output-device"
#define HL_LIST_DEVICES_SO    "l"
#define HL_LIST_DEVICES_LO    "list"
#define HL_LANG_SO            "g"
#define HL_LANG_LO            "lang"

/**
 * Args parsed from CLI flags.
 */
typedef struct HulaImmediateArgs
{
    bool startRecord = false;
    bool exit = false;
} HulaImmediateArgs;

/**
 * Print an error message about the invalid argument and exit.
 *
 * @param name Name of the option to which an invalid argument was supplied
 */
void invalidArg(QString name, QString arg, QString message = "")
{
    fprintf(stderr, "%s\n", qPrintable(CLI::tr("Invalid argument '%1' provided to option '%2'.").arg(arg, name)));
    if (!message.isEmpty())
    {
        fprintf(stderr, "%s\n", qPrintable(message));
    }
}

/**
 * Parse the command line arguments using the Qt parser.
 *
 * @param app Reference QtApplication associated with parse
 * @param extraArgs Reference to extraArgs struct where values will be updated
 * @return bool True if parse was successful. False otherwise
 */
bool parseArgsQt(QCoreApplication &app, HulaImmediateArgs &extraArgs)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(CLI::tr("Simple cross-platform audio loopback and recording."));

    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {{HL_OUT_FILE_SO, HL_OUT_FILE_LO}, CLI::tr("Path to audio output file."), CLI::tr("output filepath")},
        {{HL_DELAY_TIME_SO, HL_DELAY_TIME_LO}, CLI::tr("Duration, in seconds, of the countdown timer before record."), CLI::tr("delay")},
        {{HL_RECORD_TIME_SO, HL_RECORD_TIME_LO}, CLI::tr("Duration, in seconds, of the record."), CLI::tr("record duration")},
        {{HL_TRIGGER_RECORD_SO, HL_TRIGGER_RECORD_LO}, CLI::tr("Start the countdown/record immediately.")},
        {{HL_SAMPLE_RATE_SO, HL_SAMPLE_RATE_LO}, CLI::tr("Desired sample rate of the output file."), CLI::tr("sample rate")},
        {{HL_ENCODING_SO, HL_ENCODING_LO}, CLI::tr("Encoding format for the output file. Valid options are WAV and MP3. This will default to WAV."), CLI::tr("encoding")},
        {{HL_INPUT_DEVICE_SO, HL_INPUT_DEVICE_LO}, CLI::tr("System name of the input device. This will default if not provided."), CLI::tr("input device name")},
        {{HL_OUTPUT_DEVICE_SO, HL_OUTPUT_DEVICE_LO}, CLI::tr("System name of the output device. This will default if not provided."), CLI::tr("output device name")},
        {{HL_LIST_DEVICES_SO, HL_LIST_DEVICES_LO}, CLI::tr("List available input and output devices.")},
        {{HL_LANG_SO, HL_LANG_LO}, CLI::tr("Set the language of the application"), CLI::tr("target language")}
    });

    // This will exit if any of the args are incorrect
    parser.process(app);

    // Setup the settings module
    HulaSettings *settings = HulaSettings::getInstance();

    if (parser.isSet(HL_LANG_LO))
    {
        bool success = settings->loadLanguage(&app, parser.value(HL_LANG_LO).toStdString());
        if (success)
        {
            printf("%s\n", qPrintable(CLI::tr("Translation file successfully loaded.")));
        }
        else
        {
            fprintf(stderr, "%s\n", qPrintable(CLI::tr("Could not find translation file for %1.").arg(parser.value(HL_LANG_LO))));
        }
    }

    if (parser.isSet(HL_OUT_FILE_LO))
    {
        settings->setOutputFilePath(parser.value(HL_OUT_FILE_LO).toStdString());
    }

    if (parser.isSet(HL_DELAY_TIME_LO))
    {
        bool ok = false;
        double delay = parser.value(HL_DELAY_TIME_LO).toDouble(&ok);
        if (!ok)
        {
            invalidArg(HL_DELAY_TIME_LO, parser.value(HL_DELAY_TIME_LO));
            return false;
        }
        settings->setDelayTimer(delay);
    }

    if (parser.isSet(HL_RECORD_TIME_LO))
    {
        bool ok = false;
        double duration = parser.value(HL_RECORD_TIME_LO).toDouble(&ok);
        if (!ok)
        {
            invalidArg(HL_RECORD_TIME_LO, parser.value(HL_RECORD_TIME_LO));
            return false;
        }
        settings->setRecordDuration(duration);
    }

    if (parser.isSet(HL_TRIGGER_RECORD_LO))
    {
        extraArgs.startRecord = true;
    }

    if (parser.isSet(HL_SAMPLE_RATE_LO))
    {
        bool ok = false;
        int rate = parser.value(HL_SAMPLE_RATE_LO).toInt(&ok);
        if (!ok)
        {
            invalidArg(HL_SAMPLE_RATE_LO, parser.value(HL_SAMPLE_RATE_LO));
            return false;
        }
        settings->setSampleRate(rate);
    }

    if (parser.isSet(HL_ENCODING_LO))
    {
        std::string encoding = parser.value(HL_ENCODING_LO).toStdString();
        if (encoding == "WAV")
        {
            encoding = WAV;
        }
        else
        {
            invalidArg(HL_ENCODING_LO, parser.value(HL_ENCODING_LO), CLI::tr("Only WAV format is supported at this time."));
            return false;
        }
    }

    if (parser.isSet(HL_INPUT_DEVICE_LO))
    {
        std::string device = parser.value(HL_INPUT_DEVICE_LO).toStdString();
        // TODO: See if we can check device list here
        settings->setDefaultInputDeviceName(device);
    }

    if (parser.isSet(HL_OUTPUT_DEVICE_LO))
    {
        std::string device = parser.value(HL_OUTPUT_DEVICE_LO).toStdString();
        // TODO: See if we can check device list here
        settings->setDefaultOutputDeviceName(device);
    }

    if (parser.isSet(HL_LIST_DEVICES_LO))
    {
        Transport t;

        printf("\n-------- %s --------\n", qPrintable(CLI::tr("Device List")));
        printDeviceList(&t);

        extraArgs.exit = true;
    }

    return true;
}

#endif // END HL_CLI_ARGS_H
