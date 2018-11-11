#ifndef HL_RECORD_H
#define HL_RECORD_H

#include <hlaudio/hlaudio.h>

namespace hula
{
    class Record {

        private:
            Controller* controller;

        public:
            Record(Controller* control);
            ~Record();

            void start();
            void stop();
    };
}

#endif // END HL_RECORD_H