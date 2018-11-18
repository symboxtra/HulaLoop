#ifndef HL_CLI_COMMANDS_H
#define HL_CLI_COMMANDS_H

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

#define HL_DISCARD_SHORT "di"
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

#define HL_SYSTEM_SHORT   "sys"
#define HL_SYSTEM_LONG    "system"

#define HL_EXIT_LONG     "exit"

#define HL_HELP_TEXT     "\n"\
"Commands: \n" \
"\n" \
"  " HL_HELP_SHORT    ", " HL_HELP_LONG    "\t\t\t\t Displays this help.\n" \
"  " HL_VERSION_SHORT ", " HL_VERSION_LONG "\t\t\t\t Displays version information.\n" \
"  " HL_LIST_SHORT    ", " HL_LIST_LONG    "\t\t\t\t List all devices.\n" \
"  " HL_PRINT_SHORT   ", " HL_PRINT_LONG   "\t\t\t\t Print the current configuration.\n" \
"  " HL_EXIT_LONG                          "\t\t\t\t\t Exit HulaLoop.\n" \
"\n" \
"  " HL_RECORD_SHORT  ", " HL_RECORD_LONG " [" HL_RECORD_ARG1 "] [" HL_RECORD_ARG2 "]" \
"\t\t Start the record.\n" \
"  " HL_STOP_SHORT    ", " HL_STOP_LONG   "\t\t\t\t Stop the record or playback.\n" \
"  " HL_PLAY_SHORT    ", " HL_PLAY_LONG   "\t\t\t\t Playback captured audio.\n" \
"  " HL_PAUSE_SHORT   ", " HL_PAUSE_LONG  "\t\t\t\t Pause playback or recording.\n" \
"  " HL_EXPORT_SHORT  ", " HL_EXPORT_LONG " <" HL_EXPORT_ARG1 ">\t\t\t Export captured audio to the specified file.\n" \
"  " HL_DISCARD_SHORT ", " HL_DISCARD_SHORT " [" HL_DISCARD_ARG1 "]"\
"\t\t\t Discard the current recording\n" \
"\n" \
"  " HL_INPUT_SHORT   ", " HL_INPUT_LONG  " <" HL_INPUT_ARG1 ">\t\t\t Set the input device.\n" \
"  " HL_OUTPUT_SHORT  ", " HL_OUTPUT_LONG " <" HL_OUTPUT_ARG1 ">\t\t\t Set the output device.\n" \
"\n" \
"  " HL_DELAY_TIMER_SHORT  ", " HL_DELAY_TIMER_LONG  " <" HL_DELAY_TIMER_ARG1  ">" \
"\t\t\t Set the duration, in seconds, of the\n" \
"\t\t\t\t\t countdown timer before record. This will\n" \
"\t\t\t\t\t be overridden by an argument to the record\n" \
"\t\t\t\t\t command.\n" \
"  " HL_RECORD_TIMER_SHORT ", " HL_RECORD_TIMER_LONG " <" HL_RECORD_TIMER_ARG1 ">" \
"\t\t Set the duration, in seconds, of the\n" \
"\t\t\t\t\t record. This will be overridden by an argument\n" \
"\t\t\t\t\t to the record command.\n" \
"\n" \
"  " HL_SYSTEM_SHORT  ", " HL_SYSTEM_LONG "\t\t\t\t Run a system command from within HulaLoop.\n" \
"\n"

#endif // END HL_CLI_COMMANDS_H