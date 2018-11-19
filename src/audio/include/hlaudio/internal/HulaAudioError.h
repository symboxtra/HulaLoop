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
#define HL_SAMPLE_RATE_VALID "Sample rate and format are valid."
#define HL_SAMPLE_RATE_INVALID "The specified sample rate or format is invalid."

#define HL_OS_INIT_FAILED_CODE -1
#define HL_OS_INIT_FAILED_MSG  "Error initializing OS audio module!"

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
            int getErrorCode()
            {
                return errno;
            }
    };
}

#endif // END HL_AUDIO_ERROR_H