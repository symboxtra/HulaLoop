#ifndef RECORD_H
#define RECORD_H

#include <hlaudio/hlaudio.h>

class Record {

    private:
        Controller* controller;

    public:
        Record(Controller* control);
        ~Record();

        void start();
        void stop();
};

#endif