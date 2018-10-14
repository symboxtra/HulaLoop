#ifndef OSX_DAEMON_H
#define OSX_DAEMON_H

#include "JackClient.hpp"
#include "JackBridge.h"

class OSXDaemon : public JackClient, public JackBridgeDriverIF {
    public:

        OSXDaemon(const char *name, int id);
        virtual ~OSXDaemon();

        int process_callback(jack_nframes_t nframes) override;

    private:
        bool isActive;
        bool isSyncMode;
        bool isVerbose;
        bool showmsg;
        uint64_t lastHostTime;
        double HostTicksPerFrame;
        int64_t ncalls;

        int sendToCoreAudio(float **in, int nframes);
        int receiveFromCoreAudio(float **out, int nframes);
        void check_progress();

};

#endif // END OSX_DAEMON_H