#ifndef HULA_INTERACTIVE_CLI_H
#define HULA_INTERACTIVE_CLI_H

#include <hlcontrol/hlcontrol.h>

/**
 * Class containing the interactive CLI.
 */
class HulaInteractiveCli {
    private:
        Transport *t;

    public:
        HulaInteractiveCli();

        void unusedArgs(const std::vector<std::string> &args, int numUsed) const;
        void missingArg(const std::string &argName) const;

        void start();

        ~HulaInteractiveCli();
};

#endif // END HULA_INTERACTIVE_CLI_H
