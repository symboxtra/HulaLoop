#include "OSXDaemon.h"

int main (int argc, char ** argv)
{
    // Create the loopback daemon
    OSXDaemon * osxDaemon;
    osxDaemon = new OSXDaemon("HulaLoop #1", 0);
    osxDaemon->activate();

    // Infinite loop until daemon is killed.
    while (1)
    {
        sleep(600);
    }

    return 0;
}