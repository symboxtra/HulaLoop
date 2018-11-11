#include <thread>

#include "OSXDaemon.h"

using namespace hula;

int main (int argc, char **argv)
{
    // Create the loopback daemon
    OSXDaemon *osxDaemon;
    osxDaemon = new OSXDaemon("HulaLoop #1", 0);
    osxDaemon->activate();
    osxDaemon->monitor();

    // Infinite loop until daemon is killed.
    while (1)
    {
        std::this_thread::yield();
    }

    return 0;
}