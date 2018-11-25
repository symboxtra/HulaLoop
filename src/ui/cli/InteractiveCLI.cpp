#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "CLICommon.h"
#include "CLICommands.h"
#include "InteractiveCLI.h"

using namespace hula;

/**
 * Constuct a new instance of HulaInteractiveCli.
 *
 * This will not start the command loop.
 */
HulaInteractiveCli::HulaInteractiveCli()
{
    this->t = new Transport();
    this->settings = HulaSettings::getInstance();
}

/**
 * Print a warning to the user detailing extra arguments
 * that they provided.
 *
 * @param args Vector of all arguments provided to the command
 * @param numUsed Number of arguments actually used by the command
 */
void HulaInteractiveCli::unusedArgs(const std::vector<std::string> &args, int numUsed) const
{
    for (size_t i = numUsed - 1; i < args.size(); i++)
    {
        fprintf(stderr, "%sWarning: Unused argument '%s'.\n", HL_ERROR_PREFIX, args[i].c_str());
    }
}

/**
 * Print a warning about missing args to the specified command
 *
 * @param argName Name of the missing argument
 */
void HulaInteractiveCli::missingArg(const std::string &argName) const
{
    fprintf(stderr, "%sMissing argument '%s'\n", HL_ERROR_PREFIX, argName.c_str());
}

/**
 * Print a warning about a malformed argument.
 *
 * @param argName Name of the malformed argument
 * @param val Value given by user
 * @param type Expected type of value
 */
void HulaInteractiveCli::malformedArg(const std::string &argName, const std::string &val, const std::string &type) const
{
    fprintf(stderr, "%sMalformed argument '%s'\n", HL_ERROR_PREFIX, argName.c_str());
    fprintf(stderr, "%s'%s' is not a valid %s.\n", HL_ERROR_PREFIX, val.c_str(), type.c_str());
}

/**
 * Start taking in commands.
 *
 * This is an infinite loop.
 */
void HulaInteractiveCli::start()
{
    std::string line;
    std::string command;
    std::string arg;
    std::vector<std::string> args;

    // Command loop
    while (1)
    {
        command = "";
        arg = "";
        args.clear();

        printf(HL_PROMPT);

        // Get the whole line
        if (std::getline(std::cin, line))
        {
            // Parse the command and args out of the line
            std::istringstream iss(line);
            iss >> command;
            while (iss >> arg)
            {
                args.push_back(arg);
            }
        }
        else
        {
            fprintf(stderr, "%sEnd of stream.\n", HL_ERROR_PREFIX);
            break;
        }

        HulaCliStatus stat = processCommand(command, args);

        if (stat == HulaCliStatus::HULA_CLI_EXIT)
        {
            break;
        }
    }
}

/**
 * Proccess the command entered by the user.
 *
 * @param command Name of command in short or long form
 * @param args Vector of arguments that should be used with the command
 * @return HulaCliStatus Outcome of the command
 */
HulaCliStatus HulaInteractiveCli::processCommand(const std::string &command, const std::vector<std::string> &args)
{
    bool success = true;
    HulaCliStatus stat = HulaCliStatus::HULA_CLI_SUCCESS;

    // Take action based on the command
    if (command.size() == 0)
    {
        return HulaCliStatus::HULA_CLI_SUCCESS;
    }
    else if (command == HL_DELAY_TIMER_SHORT || command == HL_DELAY_TIMER_LONG)
    {
        // Make sure the arg exists
        if (args.size() != 0)
        {
            double delay;
            try
            {
                delay = std::stod(args[0], nullptr);
                HulaSettings::getInstance()->setDelayTimer(delay);
            }
            catch (std::invalid_argument &e)
            {
                (void)e;

                malformedArg(HL_DELAY_TIMER_ARG1, args[0], "double");
                return HulaCliStatus::HULA_CLI_FAILURE;
            }
        }
        else
        {
            missingArg(HL_DELAY_TIMER_ARG1);
            return HulaCliStatus::HULA_CLI_FAILURE;
        }
    }
    else if (command == HL_RECORD_TIMER_SHORT || command == HL_RECORD_TIMER_LONG)
    {
        // Make sure the arg exists
        if (args.size() != 0)
        {
            double duration;
            try
            {
                duration = std::stod(args[0], nullptr);
                HulaSettings::getInstance()->setRecordDuration(duration);
            }
            catch (std::invalid_argument &e)
            {
                (void)e;

                malformedArg(HL_RECORD_TIMER_ARG1, args[0], "double");
                return HulaCliStatus::HULA_CLI_FAILURE;
            }
        }
        else
        {
            missingArg(HL_RECORD_TIMER_ARG1);
            return HulaCliStatus::HULA_CLI_FAILURE;
        }
    }
    else if (command == HL_RECORD_SHORT || command == HL_RECORD_LONG)
    {
        double delay = 0;
        double duration = HL_INFINITE_RECORD;

        // Make sure the arg exists
        if (args.size() == 2)
        {
            try
            {
                delay = std::stod(args[0], nullptr);
            }
            catch (std::invalid_argument &e)
            {
                (void)e;

                malformedArg(HL_RECORD_ARG1, args[0], "double");
                return HulaCliStatus::HULA_CLI_FAILURE;
            }

            try
            {
                duration = std::stod(args[1], nullptr);
            }
            catch (std::invalid_argument &e)
            {
                (void)e;

                malformedArg(HL_RECORD_ARG2, args[1], "double");
                return HulaCliStatus::HULA_CLI_FAILURE;
            }

            success = t->record(delay, duration);
        }
        else if (args.size() == 1)
        {
            try
            {
                delay = std::stod(args[0], nullptr);
            }
            catch (std::invalid_argument &e)
            {
                (void)e;

                malformedArg(HL_RECORD_ARG1, args[0], "double");
                return HulaCliStatus::HULA_CLI_FAILURE;
            }

            success = t->record(delay, HL_INFINITE_RECORD);
        }
        else
        {
            success = t->record();
        }
    }
    else if (command == HL_STOP_SHORT || command == HL_STOP_LONG)
    {
        success = t->stop();
    }
    else if (command == HL_PLAY_SHORT || command == HL_PLAY_LONG)
    {
        success = t->play();
    }
    else if (command == HL_PAUSE_SHORT || command == HL_PAUSE_LONG)
    {
        success = t->pause();
    }
    else if (command == HL_EXPORT_SHORT || command == HL_EXPORT_LONG)
    {
        // Make sure the arg exists
        if (args.size() != 0)
        {
            t->exportFile(args[0]);
        }
        else if (settings->getOutputFilePath().size() != 0)
        {
            t->exportFile(settings->getOutputFilePath());
        }
        else
        {
            missingArg(HL_EXPORT_ARG1);
            return HulaCliStatus::HULA_CLI_FAILURE;
        }
    }
    else if (command == HL_LIST_SHORT || command == HL_LIST_LONG)
    {
        printDeviceList(t);
    }
    else if (command == HL_INPUT_SHORT || command == HL_INPUT_LONG)
    {
        Device *device = NULL;
        // Make sure the arg exists
        if (args.size() != 0)
        {
            device = findDevice(t, args[0], (DeviceType)(DeviceType::RECORD | DeviceType::LOOPBACK));
        }
        else if (settings->getDefaultInputDeviceName().size() != 0)
        {
            device = findDevice(t, settings->getDefaultInputDeviceName(), (DeviceType)(DeviceType::RECORD | DeviceType::LOOPBACK));
        }
        else
        {
            missingArg(HL_INPUT_ARG1);
            return HulaCliStatus::HULA_CLI_FAILURE;
        }

        // Find device will already have printed a not-found error
        if (device != NULL)
        {
            t->getController()->setActiveInputDevice(device);
            printf("\nInput device set to: %s\n", device->getName().c_str());

            // TODO: Settings shouldn't really store this
            settings->setDefaultInputDeviceName(device->getName());
            delete device;
        }
        else
        {
            return HulaCliStatus::HULA_CLI_FAILURE;
        }
    }
    else if (command == HL_OUTPUT_SHORT || command == HL_OUTPUT_LONG)
    {
        Device *device = NULL;
        // Make sure the arg exists
        if (args.size() != 0)
        {
            device = findDevice(t, args[0], DeviceType::PLAYBACK);
        }
        else if (settings->getDefaultOutputDeviceName().size() != 0)
        {
            device = findDevice(t, settings->getDefaultOutputDeviceName(), DeviceType::PLAYBACK);
        }
        else
        {
            missingArg(HL_OUTPUT_ARG1);
            return HulaCliStatus::HULA_CLI_FAILURE;
        }

        // Find device will already have printed a not-found error
        if (device != NULL)
        {
            t->getController()->setActiveOutputDevice(device);
            printf("\nOutput device set to: %s\n", device->getName().c_str());

            // TODO: Settings shouldn't really store this
            settings->setDefaultOutputDeviceName(device->getName());
            delete device;
        }
        else
        {
            return HulaCliStatus::HULA_CLI_FAILURE;
        }
    }
    else if (command == HL_PRINT_SHORT || command == HL_PRINT_LONG)
    {
        printSettings();
    }
    else if (command == HL_VERSION_SHORT || command == HL_VERSION_LONG)
    {
        printf("%s v%s\n", HL_CLI_NAME, HL_VERSION_STR);
    }
    else if (command == HL_HELP_SHORT || command == HL_HELP_LONG)
    {
        printf(HL_HELP_TEXT);
    }
    else if (command == HL_SYSTEM_SHORT || command == HL_SYSTEM_LONG)
    {
        // Make sure there is a command processor available
        if (system(NULL))
        {
            // Construct the sys command from args
            std::string sysCommand = "";
            for (int i = 0; i < args.size(); i++)
            {
                sysCommand += args[i] + " ";
            }

            // Run the command
            if (sysCommand.size() > 0)
            {
                int ret = system(sysCommand.c_str());
                printf("\n%sSystem command returned: %d\n", HL_PRINT_PREFIX, ret);
            }
        }
        else
        {
            fprintf(stderr, "%sNo system command processor is available is available.\n", HL_ERROR_PREFIX);
            return HulaCliStatus::HULA_CLI_FAILURE;
        }
    }
    else if (command == HL_EXIT_LONG)
    {
        return HulaCliStatus::HULA_CLI_EXIT;
    }
    else
    {
        fprintf(stderr, "%sUnrecognized command '%s'\n", HL_ERROR_PREFIX, command.c_str());
        return HulaCliStatus::HULA_CLI_FAILURE;
    }

    // Make sure transport commands succeeded
    if (!success)
    {
        fprintf(stderr, "Command failed with value of 'false'.\n");
        return HulaCliStatus::HULA_CLI_FAILURE;
    }

    return HulaCliStatus::HULA_CLI_SUCCESS;
}

/**
 * Destroy the Hula Interactive Cli:: Hula Interactive Cli object
 *
 * @return State of the transport
 */
TransportState HulaInteractiveCli::getState()
{
    return this->t->getState();
}

/**
 * Destructor for HulaInteractiveCli.
 */
HulaInteractiveCli::~HulaInteractiveCli()
{
    fprintf(stderr, "%sHulaInteractiveCli destructor called.\n", HL_ERROR_PREFIX);

    delete this->t;
}
