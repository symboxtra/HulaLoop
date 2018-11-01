#ifndef HULA_COMMANDS_H
#define HULA_COMMANDS_H

#define HL_ORANGE "\033[48;2;255;165;0m"
#define HL_NC     "\033[0m"

#define HL_PROMPT "\nHulaLoop> "

#define HL_RECORD_TIMER_LONG   "rtimer"
#define HL_RECORD_TIMER_SHORT  "rt"
#define HL_RECORD_TIMER_ARG1   "duration"

#define HL_DELAY_TIMER_LONG    "dtimer"
#define HL_DELAY_TIMER_SHORT   "dt"
#define HL_DELAY_TIMER_ARG1    "length"

#define HL_RECORD_SHORT  "r"
#define HL_RECORD_LONG   "record"

#define HL_STOP_SHORT    "s"
#define HL_STOP_LONG     "stop"

#define HL_PLAY_SHORT    "pl"
#define HL_PLAY_LONG     "play"

#define HL_PAUSE_SHORT   "pa"
#define HL_PAUSE_LONG    "pause"

#define HL_EXPORT_SHORT  "e"
#define HL_EXPORT_LONG   "export"
#define HL_EXPORT_ARG1   "filepath"

#define HL_LIST_SHORT    "l"
#define HL_LIST_LONG     "list"

#define HL_HELP_SHORT    "h"
#define HL_HELP_LONG     "help"
#define HL_HELP_TEXT     ""\
"Commands: \n" \
"\n" \
"  h, help                                Displays this help.\n" \
"  v, version                             Displays version information.\n" \
"  f, out-file <out-file-path>            Path to audio output file.\n" \
"  d, delay <delay>                       Duration, in seconds, of the\n" \
"                                         countdown timer before record.\n" \
"  t, time <record duration>              Duration, in seconds, of the\n" \
"                                         record.\n" \
"  r, record                              Start the countdown/record\n" \
"                                         immediately.\n" \
"  s, sample-rate <sample rate>           Desired sample rate of the output\n" \
"                                         file.\n" \
"  e, encoding <encoding>                 Encoding format for the output\n" \
"                                         file. Valid options are WAV and MP3.\n" \
"                                         This will default to WAV.\n" \
"  i, input-device <input-device-name>    System name of the input device.\n" \
"                                         This will default if not provided.\n" \
"  o, output-device <output-device-name>  System name of the output device.\n" \
"                                         This will default if not provided.\n" \
"  l, list                                List available input and output\n" \
"                                         devices.\n" \

#define HL_SYSTEM_SHORT   "sys"
#define HL_SYSTEM_LONG    "system"

#define HL_EXIT_LONG     "exit"

#endif // END HULA_COMMANDS_H