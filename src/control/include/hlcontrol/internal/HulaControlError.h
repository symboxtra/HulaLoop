#ifndef HL_CONTROL_ERROR_H
#define HL_CONTROL_ERROR_H

#include <string>

#include <hlaudio/internal/HulaAudioError.h>

#include <QCoreApplication>
#include <QString>

/**
 * @file HulaControlError.h <hlcontrol/internal/HulaControlError.h>
 * @ingroup public_api
 *
 * This file is automatically included via @ref hlcontrol.h
 *
 * The exceptions defined in this file are those related to the control
 * module. Higher and lower level modules have their own error files which
 * define additional exceptions.
 *
 */

/******************************************************************************
 *
 * Collection of error codes and exceptions for hlcontrol
 *
******************************************************************************/
namespace hula
{
    inline QString getTranslatedErrorMessage(int);

    /**
     * Exception class for the control module and higher.
     */
    class ControlException {

        Q_DECLARE_TR_FUNCTIONS(Exception)

        private:
            int errorCode;

        public:
            ControlException(int errorCode)
            {
                this->errorCode = errorCode;
            }

            const std::string getMessage() const
            {
                return std::string(qPrintable(getTranslatedErrorMessage(errorCode)));
            }

            int getErrorCode()
            {
                return errorCode;
            }
    };

    /**
     * Retrieve the description associated with a given error code.
     * These descriptions are translated.
     *
     * This method helps keep Qt out of the audio library.
     * Any exception coming from the audio library will contain an
     * error code and an English message.
     *
     * This function should be used to retrieve the translation.
     *
     * @return QString Translated description of error message
     */
    inline QString getTranslatedErrorMessage(int code)
    {
        switch (code)
        {
            case HL_OS_INIT_FAILED_CODE:
                return ControlException::tr(HL_OS_INIT_FAILED_MSG);
                break;
            default:
                return QString(ControlException::tr("Unknown error code: %1").arg(code));
                break;
        }
    }
}

#endif // END HL_CONTROL_ERROR_H