#include "HulaInteractiveCli.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
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
        else if (command == RECORD_SHORT || command == RECORD_LONG)
        {
            t->record();
        }
        else if (command == STOP_SHORT || command == STOP_LONG)
        {
            t->stop();
        }
        else if (command == PLAY_SHORT || command == PLAY_LONG)
        {
            t->play();
        }
        else if (command == PAUSE_SHORT || command == PAUSE_LONG)
        {
            t->pause();
        }
        else if (command == EXPORT_SHORT || command == EXPORT_LONG)
        {
            // Make sure the arg exists
            if (args.size() != 0 && args[0].size() != 0)
            {
                t->exportFile(arg);
            }
            else
            {
                missingArg(EXPORT_ARG1);
            }
        }
        else if (command == SYSTEM_SHORT || command == SYSTEM_LONG)
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
        else if (command == EXIT_LONG)
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
