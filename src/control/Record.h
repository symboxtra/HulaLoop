#ifndef RECORD_H
#define RECORD_H

#include "Controller.h"
#include "ICallback.h"

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