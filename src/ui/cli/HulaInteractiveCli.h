#ifndef HULA_INTERACTIVE_CLI_H
#define HULA_INTERACTIVE_CLI_H

#include <hlcontrol/hlcontrol.h>

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

        ~HulaInteractiveCli();
};

#endif // END HULA_INTERACTIVE_CLI_H
