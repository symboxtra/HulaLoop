#ifndef HL_RECORD
#define HL_RECORD

#include <thread>

#include <hlaudio/hlaudio.h>

namespace hula
{
class Record {

    private:
        Controller *controller;
        HulaRingBuffer *rb;

        thread recordThread;

        atomic<bool> endRecord;

        vector<std::string> exportPaths;

    public:
        Record(Controller *control);
        ~Record();

        void recorder();

        vector<std::string> getExportPaths();
        void clearExportPaths();

        void start();
        void stop();
};
}

#endif // END HL_RECORD_H