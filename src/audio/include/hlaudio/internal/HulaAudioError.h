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

#define hlDebug() \
    if (HL_NO_DEBUG_OUTPUT) {} \
    else std::cerr << HL_PRINT_PREFIX

#define hlDebugf(...) \
    if (HL_NO_DEBUG_OUTPUT) {} \
    else fprintf(stderr, HL_PRINT_PREFIX __VA_ARGS__);

/******************************************************************************
 *
 * Collection of exceptions for hlaudio
 *
******************************************************************************/
#define HL_SAMPLE_RATE_VALID "Sample rate and format are valid."
#define HL_SAMPLE_RATE_INVALID "The specified sample rate or format is invalid."

namespace hula
{

}

#endif // END HL_AUDIO_ERROR_H