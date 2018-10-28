#include "hlcontrol/internal/Record.h"

Record::Record(Controller* control)
{
    this->controller = control;
    this->rb = NULL;
}

void Record::start()
{
    if(this->rb == NULL)
        this->rb = this->controller->createAndAddBuffer(4);

    this->endRecord.store(false);
    recordThread = thread(&Record::recorder, this);
}

void Record::recorder(Record* _this)
{
    uint32_t samplesRead;
    float buffer[1024];

    while(!_this->endRecord.load())
    {
        samplesRead = _this->rb->read(buffer, 1024);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void Record::stop()
{
    this->endRecord.store(true);

    if(recordThread.joinable())
        recordThread.join();
}

Record::~Record()
{
    delete rb;
}