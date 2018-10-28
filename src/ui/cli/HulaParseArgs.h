#ifndef HULA_PARSE_ARGS_H
#define HULA_PARSE_ARGS_H

#include <QCommandLineParser>

#include <hlcontrol/hlcontrol.h>

/**
 * Parse the command line arguments using the Qt parser.
 *
 */
bool parseArgsQt(QCoreApplication &app, HulaSettings *settings)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Simple cross-platform audio loopback and recording.");

    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {{"f", "out-file"}, QCoreApplication::translate("main", "Path to audio output file.")},
        {{"d", "delay"}, QCoreApplication::translate("main", "Duration, in seconds, of the countdown timer before record.")},
        {{"t", "time"}, QCoreApplication::translate("main", "Duration, in seconds, of the record.")},
        {{"r", "record"}, QCoreApplication::translate("main", "Start the countdown/record immediately.")},
        {{"s", "sample-rate"}, QCoreApplication::translate("main", "Desired sample rate of the output file.")},
        //{{"b", "bit-depth"}, QCoreApplication::translate("main", "Sample format for the output file. Valid options are 8, 8u, 16, 16u, 32, 32u, 32f. This will default to 32f.")},
        {{"e", "encoding"}, QCoreApplication::translate("main", "Encoding format for the output file. Valid options are WAV and MP3. This will default to WAV.")},
        {{"i", "input-device"}, QCoreApplication::translate("main", "System name of the input device. This will default if not provided.")},
        {{"o", "output-device"}, QCoreApplication::translate("main", "System name of the output device. This will default if not provided.")},
        {{"l", "list"}, QCoreApplication::translate("main", "List available input and output devices.")}

        // -d --delay  Countdown timer before record.
        // -l --length Record duration.
        // -r --record Start recording/countdown timer immediately
        // -s --sample-rate Sample rate
        // -e --encoding Encoding format: valid formats WAV, MP3, guess from file, default to WAV
        // -i --input-device System name of input device, will default
        // -o --output-device System name of output device, will default
    });

    parser.process(app);

    return true;
}

#endif // END HULA_PARSE_ARGS_H
