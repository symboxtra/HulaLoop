#ifndef HL_AUDIO_ERROR_H
#define HL_AUDIO_ERROR_H

#include <cstdio>
#include <iostream>

#include "HulaVersion.h"

/**
 * @file HulaAudioError.h <hlaudio/internal/HulaAudioError.h>
 * @ingroup public_api
 *
 * This file is automatically included via @ref hlaudio.h
 *
 * This is the base file for all error handling in the application.
 * Since any module higher than this must include this file, you'll find
 * any global error information here.
 *
 * The exceptions defined in this file are those related to the audio
 * module. Higher level modules have their own error file which defines
 * additional exceptions.
 *
 */

/******************************************************************************
 *
 * Collection of global information for HulaLoop
 *
******************************************************************************/

/**
 * Prefix printed before every console message to stdout.
 */
#define HL_PRINT_PREFIX "[HulaLoop] "

/**
 * Prefix printed before every console message to stderr.
 */
#define HL_ERROR_PREFIX "[HulaLoop] "

/**
 * Debug macro that prefixes each debug statement with the HulaLoop prefix.
 */
#define hlDebug() \
    if (HL_NO_DEBUG_OUTPUT) {} \
    else std::cerr << HL_PRINT_PREFIX

/**
 * Debug macro that prefixes each debug statement with the HulaLoop prefix.
 */
#define hlDebugf(...) \
    if (HL_NO_DEBUG_OUTPUT) {} \
    else fprintf(stderr, HL_PRINT_PREFIX __VA_ARGS__);

/******************************************************************************
 *
 * Collection of error codes and exceptions for hlaudio
 *
******************************************************************************/

// OSAudio error messages
#define HL_OS_INIT_CODE -1
#define HL_OS_INIT_MSG  "Could not initialize OS audio module!"

#define HL_PA_INIT_CODE -2
#define HL_PA_INIT_MSG  "Could not initialize Port Audio!"

#define HL_PA_GET_DEVICES_CODE -3
#define HL_PA_GET_DEVICES_MSG  "Could not fetch Port Audio devices!"

#define HL_PA_OPEN_STREAM_CODE -4
#define HL_PA_OPEN_STREAM_MSG  "Could not open Port Audio device stream!"

#define HL_PA_CLOSE_STREAM_CODE -5
#define HL_PA_CLOSE_STREAM_MSG  "Could not close Port Audio device stream!"

#define HL_DEVICE_NOT_FOUND_CODE -6
#define HL_DEVICE_NOT_FOUND_MSG  "Device not found!"

#define HL_CHECK_PARAMS_CODE -7
#define HL_CHECK_PARAMS_MSG  "The specified sample rate or format is invalid!"

#define HL_PA_DEVICE_READ_STREAM_CODE -8
#define HL_PA_DEVICE_READ_STREAM_MSG  "Error during read from device stream!"

#define HL_INPUT_EQUAL_OUTPUT_CODE -9
#define HL_INPUT_EQUAL_OUTPUT_MSG  "Cannot assign an output device as input!"

#define HL_OUTPUT_EQUAL_INPUT_CODE -10
#define HL_OUTPUT_EQUAL_INPUT_MSG  "Cannot assign an input device as output!"

// OSXAudio error messages
// Block: 100-109
#define HL_OSX_DAEMON_INIT_CODE -100
#define HL_OSX_DAEMON_INIT_MSG  "Could not start hulaloop-osx-daemon process!"

#define HL_OSX_DAEMON_CRASH_CODE -101
#define HL_OSX_DAEMON_CRASH_MSG  "The hulaloop-osx-daemon process crashed!"

#define HL_OSX_PGREP_CODE -102
#define HL_OSX_PGREP_MSG  "Could not start pgrep process!"

#define HL_OSX_EXEPATH_CODE -103
#define HL_OSX_EXEPATH_MSG  "Could not retrieve path to current executable!"

#define HL_OSX_EXEPATH_TRIM_CODE -104
#define HL_OSX_EXEPATH_TRIM_MSG  "Could not trim executable name from install path!"

// LinuxAudio error messages
// Block: 110-119
#define HL_LINUX_OPEN_DEVICE_CODE -110
#define HL_LINUX_OPEN_DEVICE_MSG  "Could not open PulseAudio audio device!"

#define HL_LINUX_CLOSE_STREAM_CODE -111
#define HL_LINUX_CLOSE_STREAM_MSG  "Could not close PulseAudio device stream!"

#define HL_LINUX_SET_PARAMS_CODE -112
#define HL_LINUX_SET_PARAMS_MSG "Could not set PulseAudio device params!"

// WindowsAudio error messages
// Block: 120-229
#define HL_WIN_GET_DEVICES_CODE -120
#define HL_WIN_GET_DEVICES_MSG "Could not fetch WASAPI audio devices!"

#define HL_WIN_OPEN_STREAM_CODE -121
#define HL_WIN_OPEN_STREAM_MSG  "Could not open WASAPI device stream!"

// HulaRingBuffer error messages
#define HL_RB_ALLOC_BUFFER_CODE -200
#define HL_RB_ALLOC_BUFFER_MSG  "Could not allocate ring buffer!"

#define HL_RB_INIT_BUFFER_CODE -201
#define HL_RB_INIT_BUFFER_MSG  "Could not initialize ring buffer! Perhaps the size is not power of 2?"

namespace hula
{
    /**
     * Exception class for the control module and higher.
     */
    class AudioException {

        private:
            int errorCode;
            std::string msg;

        public:
            /**
             * Constructor of AudioException.
             */
            AudioException(int errorCode, const std::string &msg)
            {
                this->errorCode = errorCode;
                this->msg = msg;
            }

            /**
             * This method returns the error message that corresponds to the error code.
             *
             * @return std::string - The error message
             */
            const std::string getMessage() const
            {
                return msg;
            }

            /**
             * This method returns the error code.
             *
             * @return int - The error code
             */
            int getErrorCode() const
            {
                return errorCode;
            }
    };
}

#endif // END HL_AUDIO_ERROR_H