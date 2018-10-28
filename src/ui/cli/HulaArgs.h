#ifndef HULA_PARSE_ARGS_H
#define HULA_PARSE_ARGS_H

#include <QCommandLineParser>
#include <QDebug>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

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

    QCoreApplication::quit();
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
        {{"f", "out-file"}, QCoreApplication::translate("main", "Path to audio output file."), QCoreApplication::translate("main", "out-file-path")},
        {{"d", "delay"}, QCoreApplication::translate("main", "Duration, in seconds, of the countdown timer before record."), QCoreApplication::translate("main", "delay")},
        {{"t", "time"}, QCoreApplication::translate("main", "Duration, in seconds, of the record."), QCoreApplication::translate("main", "time")},
        {{"r", "record"}, QCoreApplication::translate("main", "Start the countdown/record immediately.")},
        {{"s", "sample-rate"}, QCoreApplication::translate("main", "Desired sample rate of the output file.")},
        //{{"b", "bit-depth"}, QCoreApplication::translate("main", "Sample format for the output file. Valid options are 8, 8u, 16, 16u, 32, 32u, 32f. This will default to 32f.")},
        {{"e", "encoding"}, QCoreApplication::translate("main", "Encoding format for the output file. Valid options are WAV and MP3. This will default to WAV."), QCoreApplication::translate("main", "encoding")},
        {{"i", "input-device"}, QCoreApplication::translate("main", "System name of the input device. This will default if not provided."), QCoreApplication::translate("main", "input-device-name")},
        {{"o", "output-device"}, QCoreApplication::translate("main", "System name of the output device. This will default if not provided."), QCoreApplication::translate("main", "output-device-name")},
        {{"l", "list"}, QCoreApplication::translate("main", "List available input and output devices.")}

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

    if (parser.isSet("out-file"))
    {
        settings->setOutputFilePath(parser.value("out-file").toStdString());
    }

    if (parser.isSet("delay"))
    {
        bool ok = false;
        double delay = parser.value("delay").toDouble(&ok);
        if (!ok)
        {
            invalidArg("delay", parser.value("out-file"));
        }
        settings->setDelayTimer(delay);
    }

    if (parser.isSet("time"))
    {
        bool ok = false;
        double time = parser.value("time").toDouble(&ok);
        if (!ok)
        {
            invalidArg("time", parser.value("time"));
        }
        settings->setRecordDuration(time);
    }

    if (parser.isSet("record"))
    {
        extraArgs.startRecord = true;
    }

    if (parser.isSet("sample-rate"))
    {
        bool ok = false;
        int rate = parser.value("sample-rate").toInt(&ok);
        if (!ok)
        {
            invalidArg("sample-rate", parser.value("sample-rate"));
        }
        settings->setSampleRate(rate);
    }

    if (parser.isSet("encoding"))
    {
        std::string encoding = parser.value("encoding").toStdString();
        if (encoding == "WAV")
        {
            encoding = WAV;
        }
        else
        {
            invalidArg("encoding", parser.value("encoding"), "Only WAV format is supported at this time.");
        }
    }

    if (parser.isSet("input-device"))
    {
        std::string device = parser.value("input-device").toStdString();
        // TODO: See if we can check device list here
        settings->setDefaultInputDeviceName(device);
    }

    if (parser.isSet("output-device"))
    {
        std::string device = parser.value("output-device").toStdString();
        // TODO: See if we can check device list here
        settings->setDefaultOutputDeviceName(device);
    }

    if (parser.isSet("list"))
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

#endif // END HULA_PARSE_ARGS_H
