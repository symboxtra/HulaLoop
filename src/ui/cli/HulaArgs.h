#ifndef HULA_ARGS_H
#define HULA_ARGS_H

#include <cstdio>
#include <QCommandLineParser>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

/**
 * Define the long opt strings
 * These are used in multiple places, so defining
 * will make them easier to replace
 */
#define HL_OUT_FILE_LO        "out-file"
#define HL_DELAY_TIME_LO      "delay"
#define HL_RECORD_TIME_LO     "time"
#define HL_TRIGGER_RECORD_LO  "record"
#define HL_SAMPLE_RATE_LO     "sample-rate"
#define HL_ENCODING_LO        "encoding"
#define HL_INPUT_DEVICE_LO    "input-device"
#define HL_OUTPUT_DEVICE_LO   "output-device"
#define HL_LIST_DEVICES_LO    "list"

typedef struct HulaImmediateArgs
{
    bool startRecord = false;
} HulaImmediateArgs;

/**
 * Print an error message about the invalid argument and exit.
 *
 * @param name Name of the option to which an invalid argument was supplied
 */
void invalidArg(std::string name, QString arg, std::string message = "")
{
    fprintf(stderr, "Invalid argument '%s' provided to option '%s'\n", arg.toStdString().c_str(), name.c_str());
    if (message.size() > 0)
    {
        fprintf(stderr, "%s\n", message.c_str());
    }

    exit(1);
}

/**
 * Print the accepted arguments in a nice format.
 *
 * @param settings Settings module to print
 */
void printArgs(HulaSettings *settings)
{
    printf("Output file:          '%s'\n", settings->getOutputFilePath().c_str());
    printf("Delay:                %.2f s\n", settings->getDelayTimer());
    printf("Length:               %.2f s\n", settings->getRecordDuration());
    printf("Sample rate:          %'d Hz\n", settings->getSampleRate());
    printf("Encoding:             %s\n", "WAV");
    printf("Input device:         '%s'\n", settings->getDefaultInputDeviceName().c_str());
    printf("Output device:        '%s'\n", settings->getDefaultOutputDeviceName().c_str());
    printf("\n");
}

/**
 * Parse the command line arguments using the Qt parser.
 *
 */
HulaSettings * parseArgsQt(QCoreApplication &app, HulaImmediateArgs &extraArgs)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Simple cross-platform audio loopback and recording.");

    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {{"f", HL_OUT_FILE_LO}, QCoreApplication::translate("main", "Path to audio output file."), QCoreApplication::translate("main", "out-file-path")},
        {{"d", HL_DELAY_TIME_LO}, QCoreApplication::translate("main", "Duration, in seconds, of the countdown timer before record."), QCoreApplication::translate("main", "delay")},
        {{"t", HL_RECORD_TIME_LO}, QCoreApplication::translate("main", "Duration, in seconds, of the record."), QCoreApplication::translate("main", "record duration")},
        {{"r", HL_TRIGGER_RECORD_LO}, QCoreApplication::translate("main", "Start the countdown/record immediately.")},
        {{"s", HL_SAMPLE_RATE_LO}, QCoreApplication::translate("main", "Desired sample rate of the output file."), QCoreApplication::translate("main", "sample rate")},
        //{{"b", "bit-depth"}, QCoreApplication::translate("main", "Sample format for the output file. Valid options are 8, 8u, 16, 16u, 32, 32u, 32f. This will default to 32f.")},
        {{"e", HL_ENCODING_LO}, QCoreApplication::translate("main", "Encoding format for the output file. Valid options are WAV and MP3. This will default to WAV."), QCoreApplication::translate("main", "encoding")},
        {{"i", HL_INPUT_DEVICE_LO}, QCoreApplication::translate("main", "System name of the input device. This will default if not provided."), QCoreApplication::translate("main", "input-device-name")},
        {{"o", HL_OUTPUT_DEVICE_LO}, QCoreApplication::translate("main", "System name of the output device. This will default if not provided."), QCoreApplication::translate("main", "output-device-name")},
        {{"l", HL_LIST_DEVICES_LO}, QCoreApplication::translate("main", "List available input and output devices.")}

        // -d --delay  Countdown timer before record.
        // -l --length Record duration.
        // -r --record Start recording/countdown timer immediately
        // -s --sample-rate Sample rate
        // -e --encoding Encoding format: valid formats WAV, MP3, guess from file, default to WAV
        // -i --input-device System name of input device, will default
        // -o --output-device System name of output device, will default
    });

    // This will exit if any of the args are incorrect
    parser.process(app);

    // Setup the settings module
    HulaSettings *settings = HulaSettings::getInstance();

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
            invalidArg(HL_DELAY_TIME_LO, parser.value("out-file"));
        }
        settings->setDelayTimer(delay);
    }

    if (parser.isSet(HL_RECORD_TIME_LO))
    {
        bool ok = false;
        double time = parser.value(HL_RECORD_TIME_LO).toDouble(&ok);
        if (!ok)
        {
            invalidArg(HL_RECORD_TIME_LO, parser.value(HL_RECORD_TIME_LO));
        }
        settings->setRecordDuration(time);
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
            invalidArg(HL_ENCODING_LO, parser.value(HL_ENCODING_LO), "Only WAV format is supported at this time.");
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
        vector<Device *> devices;
        if (settings->getShowRecordDevices())
        {
            // TODO:
            // devices = t.getController()->getDevices((DeviceType)(PLAYBACK | LOOPBACK | RECORD));
        }
        else
        {
            // devices = t.getController()->getDevices((DeviceType)(PLAYBACK | LOOPBACK));
        }
    }

    return settings;
}

#endif // END HULA_ARGS_H
