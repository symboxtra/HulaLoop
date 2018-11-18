#include "hlcontrol/internal/Playback.h"

using namespace hula;

/**
 * @brief Construct a new Playback instance to replay the captured audio data
 *
 * @param control Controller instance that will be used to interact with the ringbuffer
 */
Playback::Playback(Controller *control)
{
    this->controller = control;
}

/**
 * @brief Starts the playback of audio data from the specified time of the capture duration
 *
 * @param initTime Begin time of the capture duration (in seconds)
 */
void Playback::start(uint32_t startTime)
{

}

/**
 * @brief Stop the playback of audio data by removing ringbuffer from Controller
 *
 */
void Playback::stop()
{

}

Playback::~Playback()
{

}