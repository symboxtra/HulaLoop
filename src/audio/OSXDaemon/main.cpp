#include <signal.h>

#include <cstdio>
#include <thread>

#include "OSXDaemon.h"

using namespace hula;

bool shouldExit = false;
void handleSignal(int sig)
{
    if (sig == SIGTERM || sig == SIGQUIT || sig == SIGINT)
    {
        printf("\nCaught signal %s.\n", strsignal(sig));
        printf("Exiting...\n");
        shouldExit = true;
    }
}

int main (int argc, char **argv)
{
    // Set up the signal handlers
    struct sigaction sa;
    sa.sa_handler = handleSignal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGTERM, &sa, NULL))
    {
        perror("sigaction for SIGTERM");
    }

    if (sigaction(SIGQUIT, &sa, NULL))
    {
        perror("sigaction for SIGQUIT");
    }

    if (sigaction(SIGINT, &sa, NULL))
    {
        perror("sigaction for SIGINT");
    }

    bool foreground = false;
    if (argc > 1)
    {
        if (strcmp(argv[1], "-f") == 0)
        {
            foreground = true;
            printf("Running daemon in foreground...\n");
        }
        else
        {
            printf("Unknown argument %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    if (!foreground)
    {
        daemon(0, 0);
    }

    // Create the loopback daemon
    OSXDaemon osxDaemon("HulaLoop #1", 0);
    osxDaemon.activate();
    osxDaemon.monitor();

    // Infinite loop until daemon is killed.
    while (!shouldExit)
    {
        std::this_thread::yield();
    }

    return 0;
}