#ifndef HL_RECORD
#define HL_RECORD

#include <thread>

#include <hlaudio/hlaudio.h>

using namespace std;

class Record {

    private:
        Controller* controller;
        HulaRingBuffer* rb;

        thread recordThread;

        atomic<bool> endRecord;

    public:
        Record(Controller* control);
        ~Record();

        void recorder();

        void start();
        void stop();
};

#endif