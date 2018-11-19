#ifndef HL_CLI_COMMANDS_H
#define HL_CLI_COMMANDS_H

#include <iomanip>
#include <iostream>

#include "CLICommon.h"

#define HL_ORANGE "\033[48;2;255;165;0m"
#define HL_NC     "\033[0m"

#define HL_PROMPT "\nHulaLoop> "

#define HL_DELAY_TIMER_LONG    "delay"
#define HL_DELAY_TIMER_SHORT   "de"
#define HL_DELAY_TIMER_ARG1    "seconds"

#define HL_RECORD_TIMER_LONG   "duration"
#define HL_RECORD_TIMER_SHORT  "du"
#define HL_RECORD_TIMER_ARG1   "seconds"

#define HL_RECORD_SHORT  "r"
#define HL_RECORD_LONG   "record"
#define HL_RECORD_ARG1   "delay"
#define HL_RECORD_ARG2   "duration"

#define HL_STOP_SHORT    "s"
#define HL_STOP_LONG     "stop"

#define HL_PLAY_SHORT    "pl"
#define HL_PLAY_LONG     "play"

#define HL_PAUSE_SHORT   "pa"
#define HL_PAUSE_LONG    "pause"

#define HL_EXPORT_SHORT  "e"
#define HL_EXPORT_LONG   "export"
#define HL_EXPORT_ARG1   "filepath"

#define HL_DISCARD_SHORT "d"
#define HL_DISCARD_LONG  "discard"
#define HL_DISCARD_ARG1  "-f"

#define HL_LIST_SHORT    "l"
#define HL_LIST_LONG     "list"

#define HL_INPUT_SHORT   "i"
#define HL_INPUT_LONG    "input"
#define HL_INPUT_ARG1    "name|id"

#define HL_OUTPUT_SHORT  "o"
#define HL_OUTPUT_LONG   "output"
#define HL_OUTPUT_ARG1   "name|id"

#define HL_PRINT_SHORT   "p"
#define HL_PRINT_LONG    "print"

#define HL_VERSION_SHORT "v"
#define HL_VERSION_LONG  "version"

#define HL_HELP_SHORT    "h"
#define HL_HELP_LONG     "help"

#define HL_LANG_SHORT     "lang"
#define HL_LANG_LONG      "language"
#define HL_LANG_ARG1      "target language"

#define HL_SYSTEM_SHORT   "sys"
#define HL_SYSTEM_LONG    "system"

#define HL_EXIT_LONG     "exit"

#define C1 "  " << left << setw(5)
#define C2 setw(34)

inline void printInteractiveHelp()
{
    using std::cout;
    using std::endl;
    using std::left;
    using std::setw;

    cout << endl << "Commands:" << endl;
    cout << C1 << HL_HELP_SHORT     ", " << C2 << HL_HELP_LONG    << qPrintable(CLI::tr("Display this help.")) << endl;
    cout << C1 << HL_VERSION_SHORT  ", " << C2 << HL_VERSION_LONG << qPrintable(CLI::tr("Display version information.")) << endl;
    cout << C1 << HL_LIST_SHORT     ", " << C2 << HL_LIST_LONG    << qPrintable(CLI::tr("List all devices.")) << endl;
    cout << C1 << HL_PRINT_SHORT    ", " << C2 << HL_PRINT_LONG   << qPrintable(CLI::tr("Print the current configuration.")) << endl;
    cout << C1 << HL_LANG_SHORT << C2 << "<" HL_LANG_ARG1 "> "    << qPrintable(CLI::tr("Switch the application language.")) << endl;
    cout << C1 << HL_EXIT_LONG  << C2 << " " << qPrintable(CLI::tr("Quit the application.")) << endl;
    cout << endl;

    cout << C1 << HL_RECORD_SHORT  ", " << C2 << HL_RECORD_LONG " [" HL_RECORD_ARG1 "] [" HL_RECORD_ARG2 "]";
    cout << qPrintable(CLI::tr("Start recording.")) << endl;
    cout << C1 << HL_STOP_SHORT    ", " << C2 << HL_STOP_LONG    << qPrintable(CLI::tr("Stop playback or recording.")) << endl;
    cout << C1 << HL_PLAY_SHORT    ", " << C2 << HL_PLAY_LONG    << qPrintable(CLI::tr("Playback captured audio.")) << endl;
    cout << C1 << HL_PAUSE_SHORT   ", " << C2 << HL_PAUSE_LONG   << qPrintable(CLI::tr("Pause playback or recording.")) << endl;
    cout << C1 << HL_EXPORT_SHORT  ", " << C2 << HL_EXPORT_LONG  " <" HL_EXPORT_ARG1  "> " << qPrintable(CLI::tr("Export captured audio to the specified file.")) << endl;
    cout << C1 << HL_DISCARD_SHORT ", " << C2 << HL_DISCARD_LONG " [" HL_DISCARD_ARG1 "] " << qPrintable(CLI::tr("Discard the current recording.")) << endl;
    cout << endl;

    cout << C1 << HL_INPUT_SHORT   ", " << C2 << HL_INPUT_LONG   " <" HL_INPUT_ARG1  "> ";
    cout << qPrintable(CLI::tr("Set the input device.")) << endl;
    cout << C1 << HL_OUTPUT_SHORT  ", " << C2 << HL_OUTPUT_LONG  " <" HL_OUTPUT_ARG1 "> ";
    cout << qPrintable(CLI::tr("Set the output device.")) << endl;
    cout << endl;

    cout << C1 << HL_DELAY_TIMER_SHORT ", " << C2 << HL_DELAY_TIMER_LONG " <" HL_DELAY_TIMER_ARG1 "> ";
    cout << qPrintable(CLI::tr("Set the delay, in seconds, of the countdown timer before record.")) << endl;
    cout << C1 << " " << C2 << " " << qPrintable(CLI::tr("The timer will be overridden by an argument to the record command.")) << endl;

    cout << C1 << HL_RECORD_TIMER_SHORT ", " << C2 << HL_RECORD_TIMER_LONG " <" HL_RECORD_TIMER_ARG1 "> ";
    cout << qPrintable(CLI::tr("Set the duration, in seconds, of the record.")) << endl;
    cout << C1 << " " << C2 << " " << qPrintable(CLI::tr("The timer will be orverriden by an argument to the record command.")) << endl;
    cout << endl;

    cout << C1 << HL_SYSTEM_SHORT  ", " << C2 << HL_SYSTEM_LONG  << qPrintable(CLI::tr("Run a system command from within HulaLoop.")) << endl;
    cout << endl;
}

#endif // END HL_CLI_COMMANDS_H