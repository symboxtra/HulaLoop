#include "hlcontrol/internal/Record.h"

Record::Record(Controller* control)
{
    this->controller = control;
}

void Record::handleData(uint8_t* data, uint32_t size)
{

}

void Record::start()
{
    controller->addBufferReadyCallback(this);
}

void Record::stop()
{
    controller->removeBufferReadyCallback(this);
}

Record::~Record()
{

}