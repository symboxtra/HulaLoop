#include <chrono>
#include <cstdio>
#include <thread>

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
        printf("\n");
    }

    InteractiveCLI cli(&app);

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
        if (extraArgs.delay.size() > 0)
        {
            double delay = stod(extraArgs.delay);

            printf("%s\n", qPrintable(CLI::tr("Delaying for %1 seconds...").arg(delay)));
            std::this_thread::sleep_for(std::chrono::milliseconds((int)(delay * 1000)));
        }

        cli.processCommand(HL_RECORD_LONG, { extraArgs.delay, extraArgs.duration });

        if (extraArgs.duration.size() > 0)
        {
            double duration = stod(extraArgs.duration);

            printf("%s\n", qPrintable(CLI::tr("Recording for %1 seconds...").arg(duration)));
            std::this_thread::sleep_for(std::chrono::milliseconds((int)(duration * 1000)));
        }

        cli.processCommand(HL_STOP_LONG, {});

        if (extraArgs.outputFilePath.size() > 0)
        {
            cli.processCommand(HL_EXPORT_LONG, { extraArgs.outputFilePath });
        }
    }
}
