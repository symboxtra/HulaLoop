#include <cstdio>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include "HulaArgs.h"
#include "HulaCliCommon.h"
#include "HulaInteractiveCli.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("HulaLoop-CLI");
    QCoreApplication::setOrganizationName("Symboxtra Software");
    QCoreApplication::setApplicationVersion(HL_VERSION_STR); // TODO: Needs to use the version header

    HulaImmediateArgs extraArgs;
    HulaSettings *settings = parseArgsQt(app, extraArgs);

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
