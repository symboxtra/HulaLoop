#ifndef HL_RECORD_H
#define HL_RECORD_H

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

            void start();
            void stop();
    };
}

#endif // END HL_RECORD_H