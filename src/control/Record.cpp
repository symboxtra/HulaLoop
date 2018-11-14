#include "hlcontrol/internal/Record.h"

using namespace hula;

Record::Record(Controller* control)
{
    this->controller = control;
}

void Record::start()
{
}

void Record::stop()
{
}

Record::~Record()
{
    hlDebugf("Record destructor called\n");
}