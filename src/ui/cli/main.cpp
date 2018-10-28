#include <getopt.h>
#include <cstdio>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include "HulaParseArgs.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("HulaLoop-CLI");
    QCoreApplication::setOrganizationName("Symboxtra Software");
    QCoreApplication::setApplicationVersion("0.0.0"); // TODO: Needs to use the version header

    if (argc == 0)
    {
        // Enter interactive mode
    }

    HulaSettings *settings = HulaSettings::getInstance();
    bool success = parseArgsQt(app, settings);
}
