#include <cstdio>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include "CLIArgs.h"
#include "CLICommon.h"
#include "InteractiveCLI.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(HL_CLI_NAME);
    QCoreApplication::setOrganizationName("Symboxtra Software");
    QCoreApplication::setApplicationVersion(HL_VERSION_STR);

    HulaSettings *settings = HulaSettings::getInstance();
    QTranslator *trans = settings->getTranslator();

    HulaImmediateArgs extraArgs;
    bool success = parseArgsQt(app, extraArgs);

    // Error message will already have been printed
    if (!success)
    {
        return 1;
    }

    // Our work is already done
    if (extraArgs.exit)
    {
        return 0;
    }

    fprintf(stdout, "%s", HL_CLI_ASCII_HEADER);

    // Print the arguments provided
    printSettings();

    if (!extraArgs.startRecord)
    {
        HulaInteractiveCli cli(&app);
        cli.start();
    }
    else
    {
        Transport t;
        t.record();
    }

}
