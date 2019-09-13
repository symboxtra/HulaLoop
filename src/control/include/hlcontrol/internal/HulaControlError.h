#ifndef HL_CONTROL_ERROR_H
#define HL_CONTROL_ERROR_H

#include <string>

#include <hlaudio/internal/HulaAudioError.h>

#include <QCoreApplication>
#include <QMessageBox>
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
// Export error messages
#define HL_EXPORT_OPEN_FILE_CODE -18
#define HL_EXPORT_OPEN_FILE_MSG  "Could not open file %s!"

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
            /**
             * Constructor of ControlException.
             */
            ControlException(int errorCode)
            {
                this->errorCode = errorCode;
            }

            /**
             * This method returns the error message that corresponds to the error
             * code in the locale given by the application.
             *
             * @return std::string - The translated error message
             */
            const std::string getErrorMessage() const
            {
                return std::string(qPrintable(getTranslatedErrorMessage(errorCode)));
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
            case HL_OS_INIT_CODE:
                return ControlException::tr(HL_OS_INIT_MSG);
                break;
            case HL_PA_INIT_CODE:
                return ControlException::tr(HL_PA_INIT_MSG);
                break;
            case HL_INPUT_EQUAL_OUTPUT_CODE:
                return ControlException::tr(HL_INPUT_EQUAL_OUTPUT_MSG);
                break;
            case HL_OUTPUT_EQUAL_INPUT_CODE:
                return ControlException::tr(HL_OUTPUT_EQUAL_INPUT_MSG);
                break;
            case HL_OSX_DAEMON_INIT_CODE:
                return ControlException::tr(HL_OSX_DAEMON_INIT_MSG);
                break;
            case HL_OSX_DAEMON_CRASH_CODE:
                return ControlException::tr(HL_OSX_DAEMON_CRASH_MSG);
                break;
            case HL_OSX_PGREP_CODE:
                return ControlException::tr(HL_OSX_PGREP_MSG);
                break;
            case HL_OSX_EXEPATH_CODE:
                return ControlException::tr(HL_OSX_EXEPATH_MSG);
                break;
            case HL_OSX_EXEPATH_TRIM_CODE:
                return ControlException::tr(HL_OSX_EXEPATH_TRIM_MSG);
                break;
            case HL_DEVICE_NOT_FOUND_CODE:
                return ControlException::tr(HL_DEVICE_NOT_FOUND_MSG);
                break;
            case HL_PA_OPEN_STREAM_CODE:
                return ControlException::tr(HL_PA_OPEN_STREAM_MSG);
                break;
            case HL_PA_CLOSE_STREAM_CODE:
                return ControlException::tr(HL_PA_CLOSE_STREAM_MSG);
                break;
            case HL_PA_DEVICE_READ_STREAM_CODE:
                return ControlException::tr(HL_PA_DEVICE_READ_STREAM_MSG);
                break;
            case HL_PA_GET_DEVICES_CODE:
                return ControlException::tr(HL_PA_GET_DEVICES_MSG);
                break;
            case HL_CHECK_PARAMS_CODE:
                return ControlException::tr(HL_CHECK_PARAMS_MSG);
                break;
            case HL_LINUX_OPEN_DEVICE_CODE:
                return ControlException::tr(HL_LINUX_OPEN_DEVICE_MSG);
                break;
            case HL_RB_ALLOC_BUFFER_CODE:
                return ControlException::tr(HL_RB_ALLOC_BUFFER_MSG);
                break;
            case HL_RB_INIT_BUFFER_CODE:
                return ControlException::tr(HL_RB_INIT_BUFFER_MSG);
                break;
            case HL_EXPORT_OPEN_FILE_CODE:
                return ControlException::tr(HL_EXPORT_OPEN_FILE_MSG);
                break;
            default:
                return QString(ControlException::tr("Unknown error code: %1").arg(code));
                break;
        }
    }
}

#endif // END HL_CONTROL_ERROR_H