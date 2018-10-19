#ifndef RECORD_H
#define RECORD_H

#include <hlaudio/hlaudio.h>

class Record : public ICallback {

    private:
        Controller* controller;

    public:
        Record(Controller* control);
        ~Record();

        void handleData(uint8_t* data, uint32_t size);

        void start();
        void stop();
};

#endif