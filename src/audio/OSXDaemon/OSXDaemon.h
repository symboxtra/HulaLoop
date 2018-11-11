#ifndef HL_OSX_DAEMON_H
#define HL_OSX_DAEMON_H

#include <QCoreApplication>

#include "JackClient.hpp"
#include "JackBridge.h"

namespace hula
{
    class OSXDaemon : public JackClient, public JackBridgeDriverIF {

        Q_DECLARE_TR_FUNCTIONS(OSXDaemon)

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
}

#endif // END HL_OSX_DAEMON_H