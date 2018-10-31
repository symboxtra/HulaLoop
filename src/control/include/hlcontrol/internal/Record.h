#ifndef HL_RECORD
#define HL_RECORD

#include <thread>

#include <hlaudio/hlaudio.h>

using namespace std;

class Record {

    private:
        Controller *controller;
        HulaRingBuffer *rb;

        thread recordThread;

        atomic<bool> endRecord;

        std::string tempDirPath;

    public:
        Record(Controller *control, std::string path);
        ~Record();

        void recorder();

        void start();
        void stop();
};

#endif