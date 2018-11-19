#include <cstdio>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include "CLIArgs.h"
#include "CLICommands.h"
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

    // Print the banner and settings before other output
    if (!extraArgs.startRecord)
    {
        printf(HL_CLI_ASCII_HEADER);
        printSettings(extraArgs);
    }

    HulaInteractiveCli cli(&app);

    // Use the flag values and interactive CLI commands to setup state
    cli.processCommand(HL_DELAY_TIMER_LONG, { extraArgs.delay });
    cli.processCommand(HL_RECORD_TIMER_LONG, { extraArgs.duration });
    cli.setOutputFilePath(extraArgs.outputFilePath);

    if (extraArgs.inputDevice.size() > 0)
    {
        HulaCliStatus stat = cli.processCommand(HL_INPUT_LONG, { extraArgs.inputDevice });
        if (stat == HulaCliStatus::HULA_CLI_FAILURE)
        {
            return 1;
        }
    }

    if (extraArgs.outputDevice.size() > 0)
    {
        HulaCliStatus stat = cli.processCommand(HL_OUTPUT_LONG, { extraArgs.outputDevice });
        if (stat == HulaCliStatus::HULA_CLI_FAILURE)
        {
            return 1;
        }
    }

    if (!extraArgs.startRecord)
    {
        cli.start();
    }
    else
    {
        cli.processCommand(HL_RECORD_LONG, { extraArgs.delay, extraArgs.duration });
    }
}
