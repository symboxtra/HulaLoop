#include <cstdio>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include "HulaArgs.h"
#include "HulaInteractiveCli.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("HulaLoop-CLI");
    QCoreApplication::setOrganizationName("Symboxtra Software");
    QCoreApplication::setApplicationVersion("0.0.0"); // TODO: Needs to use the version header

    HulaImmediateArgs extraArgs;
    HulaSettings *settings = parseArgsQt(app, extraArgs);

    fprintf(stdout, "\n"
    "   _    _       _       _\n"
    "  | |  | |     | |     | |\n"
    "  | |__| |_   _| | __ _| |     ___   ___  _ __\n"
    "  |  __  | | | | |/ _` | |    / _ \\ / _ \\| '_ \\\n"
    "  | |  | | |_| | | (_| | |___| (_) | (_) | |_) |\n"
    "  |_|  |_|\\__,_|_|\\__,_|______\\___/ \\___/| .__/\n"
    "                                         | |\n"
    "                                         |_|\n"
    "----------------------------------------------------\n\n");

    // Print the arguments provided
    printArgs(settings);

    if (!extraArgs.startRecord)
    {
        HulaInteractiveCli cli;
    }
    else
    {
        Transport t;
        t.record();
    }

}
