#ifndef HL_PLAYBACK_H
#define HL_PLAYBACK_H

#include <hlaudio/hlaudio.h>

namespace hula
{
    class Playback {

        private:
            Controller *controller;

            std::thread playThread;
            std::atomic<bool> endPlay;

        public:
            Playback(Controller *controller);
            ~Playback();

            void player();

            void start(uint32_t initTime);
            void stop();
    };
}

#endif // END HL_PLAYBACK_H