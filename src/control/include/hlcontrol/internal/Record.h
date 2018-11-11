#ifndef HL_RECORD
#define HL_RECORD

#include <thread>

#include <hlaudio/hlaudio.h>

using namespace std;

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

        void start();
        void stop();
};
}

#endif // END HL_RECORD_H