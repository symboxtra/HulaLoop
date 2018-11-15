#include "hlcontrol/internal/Record.h"

using namespace hula;

/**
 * @brief Construct a new Record to capture audio data and store in temp file
 *
 * @param control Controller instance that will be used to interact with the ringbuffer
 */
Record::Record(Controller *control)
{
    this->controller = control;
}

/**
 * @brief Starts the capture of audio data by adding ringbuffer to Controller
 * and reading from ringbuffer
 *
 */
void Record::start()
{
}

/**
 * @brief Stops the capture of audio data by removing ringbuffer from Controller
 *
 */
void Record::stop()
{
}

/**
 * @brief Delete instance of the ringbuffer used
 *
 */
Record::~Record()
{

}