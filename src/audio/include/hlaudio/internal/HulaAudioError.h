#ifndef HL_AUDIO_ERROR_H
#define HL_AUDIO_ERROR_H

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
#define HL_PRINT_PREFIX "[HulaLoop] "
#define HL_ERROR_PREFIX "[HulaLoop] "

/******************************************************************************
 *
 * Collection of exceptions for hlaudio
 *
******************************************************************************/
namespace hula
{

}

#endif // END HL_AUDIO_ERROR_H