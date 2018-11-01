#include "HulaInteractiveCli.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "HulaCommands.h"

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
        fprintf(stderr, "%sWarning: Unused argument '%s'\n", HL_ERROR_PREFIX, args[i].c_str());
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
    fprintf(stderr, "%s'%s' is not a valid %s\n", HL_ERROR_PREFIX, val.c_str(), type.c_str());
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

        // Take action based on the command
        if (command.size() == 0)
        {
            continue;
        }
        else if (command == HL_DELAY_TIMER_SHORT || command == HL_DELAY_TIMER_LONG)
        {
            // Make sure the arg exists
            if (args.size() != 0)
            {
                int delay;
                try
                {
                    delay = std::stoi(args[0], nullptr);
                }
                catch (std::invalid_argument &e)
                {
                    malformedArg(HL_DELAY_TIMER_LONG, args[0], "int");
                }
            }
            else
            {
                missingArg(HL_DELAY_TIMER_ARG1);
            }
        }
        else if (command == HL_RECORD_TIMER_SHORT || command == HL_RECORD_TIMER_LONG)
        {
            // Make sure the arg exists
            if (args.size() != 0)
            {
                int duration;
                try
                {
                    duration = std::stoi(args[0], nullptr);
                }
                catch (std::invalid_argument &e)
                {
                    malformedArg(HL_RECORD_TIMER_LONG, args[0], "int");
                }
            }
            else
            {
                missingArg(HL_RECORD_TIMER_ARG1);
            }
        }
        else if (command == HL_RECORD_SHORT || command == HL_RECORD_LONG)
        {
            t->record();
        }
        else if (command == HL_STOP_SHORT || command == HL_STOP_LONG)
        {
            t->stop();
        }
        else if (command == HL_PLAY_SHORT || command == HL_PLAY_LONG)
        {
            t->play();
        }
        else if (command == HL_PAUSE_SHORT || command == HL_PAUSE_LONG)
        {
            t->pause();
        }
        else if (command == HL_EXPORT_SHORT || command == HL_EXPORT_LONG)
        {
            // Make sure the arg exists
            if (args.size() != 0)
            {
                t->exportFile(arg);
            }
            else if (settings->getOutputFilePath().size() != 0)
            {
                t->exportFile(settings->getOutputFilePath());
            }
            else
            {
                missingArg(HL_EXPORT_ARG1);
            }
        }
        else if (command == HL_LIST_SHORT || command == HL_LIST_LONG)
        {
            vector<Device *> devices;
            if (settings->getShowRecordDevices())
            {
                devices = t->getController()->getDevices((DeviceType)(PLAYBACK | LOOPBACK | RECORD));
            }
            else
            {
                devices = t->getController()->getDevices((DeviceType)(PLAYBACK | LOOPBACK));
            }

            printf("\n");
            for (size_t i = 0; i < devices.size(); i++)
            {
                printf("Device #%lu: %s\n", i, devices[i]->getName().c_str());
                printf("Record:   %s\n", (devices[i]->getType() & DeviceType::RECORD) ? "true" : "false");
                printf("Loopback: %s\n", (devices[i]->getType() & DeviceType::RECORD) ? "true" : "false");
                printf("Output:   %s\n", (devices[i]->getType() & DeviceType::RECORD) ? "true" : "false");
                printf("\n");
            }

            Device::deleteDevices(devices);
        }
        else if (command == HL_HELP_SHORT || command == HL_HELP_LONG)
        {
            printf("%s", HL_HELP_TEXT);
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
                int ret = system(sysCommand.c_str());
                printf("\n%sSystem command returned: %d\n", HL_PRINT_PREFIX, ret);
            }
            else
            {
                fprintf(stderr, "%sNo system command processor is available is available.\n", HL_ERROR_PREFIX);
            }
        }
        else if (command == HL_EXIT_LONG)
        {
            // TODO: Make sure nothing unsaved is happening
            break;
        }
        else
        {
            fprintf(stderr, "%sUnrecognized command '%s'\n", HL_ERROR_PREFIX, command.c_str());
        }
    }
}

/**
 * Destructor for HulaInteractiveCli.
 */
HulaInteractiveCli::~HulaInteractiveCli()
{
    fprintf(stderr, "%sHulaInteractiveCli destructor called.\n", HL_ERROR_PREFIX);

    delete this->t;
}
