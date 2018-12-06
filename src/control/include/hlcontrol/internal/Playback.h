#ifndef HL_PLAYBACK_H
#define HL_PLAYBACK_H

#include <hlaudio/hlaudio.h>
#include "Record.h"

namespace hula
{
    class Playback {

        private:
            Controller *controller;
            Record *recorder;

            std::thread playThread;
            std::atomic<bool> endPlay;

        public:
            Playback(Controller *controller, Record *record);
            ~Playback();

            void player();

            void start(uint32_t initTime);
            void stop();
    };
}

#endif // END HL_PLAYBACK_H