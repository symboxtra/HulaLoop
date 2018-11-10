#ifndef HL_INTERACTIVE_CLI_H
#define HL_INTERACTIVE_CLI_H

#include <hlcontrol/hlcontrol.h>

/**
 * Status returned by calls to processCommand.
 * Indicate any action required of the caller.
 */
enum HulaCliStatus
{
    HULA_CLI_SUCCESS,
    HULA_CLI_FAILURE,
    HULA_CLI_EXIT
};

/**
 * Class containing the interactive CLI.
 */
class HulaInteractiveCli {
    private:
        Transport *t;
        HulaSettings *settings;

    public:
        HulaInteractiveCli();

        void unusedArgs(const std::vector<std::string> &args, int numUsed) const;
        void missingArg(const std::string &argName) const;
        void malformedArg(const std::string &argName, const std::string &val, const std::string &type) const;

        void start();
        HulaCliStatus processCommand(const std::string &command, const std::vector<std::string> &args);
        TransportState getState();

        ~HulaInteractiveCli();
};

#endif // END HL_INTERACTIVE_CLI_H
