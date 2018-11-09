#include <cstdio>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include "HulaArgs.h"
#include "HulaCliCommon.h"
#include "HulaInteractiveCli.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("hulaloop-cli");
    QCoreApplication::setOrganizationName("Symboxtra Software");
    QCoreApplication::setApplicationVersion(HL_VERSION_STR); // TODO: Needs to use the version header

    HulaImmediateArgs extraArgs;
    bool success = parseArgsQt(app, extraArgs);

    // Error message will already have been printed
    if (!success)
    {
        exit(1);
    }

    // Our work is already done
    if (extraArgs.exit)
    {
        exit(0);
    }

    fprintf(stdout, "%s", HL_ASCII_HEADER);

    // Print the arguments provided
    printSettings();

    if (!extraArgs.startRecord)
    {
        HulaInteractiveCli cli;
        cli.start();
    }
    else
    {
        Transport t;
        t.record();
    }

}
