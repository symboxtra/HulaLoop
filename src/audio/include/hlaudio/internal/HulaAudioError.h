#ifndef HL_AUDIO_ERROR_H
#define HL_AUDIO_ERROR_H

#include <iostream>

#include <QtGlobal>
#include <QString>

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
 * Open a QTextStream to stdout since all qDebug* functions use stderrr
 *
 * @return QTextStream& Stream to stdout

inline QTextStream& qOut()
{
    static QTextStream ts(stdout);
    return ts;
}
*/

/**
 * Message handler for all debug and output messages.
 *
 * Based on code from http://doc.qt.io/qt-5/qtglobal.html#qInstallMessageHandler
 */
inline void qtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    switch (type)
    {
        case QtDebugMsg:
            std::cerr << "  Debug: " << msg.toUtf8().constData() << std::endl;
            break;
        case QtInfoMsg:
            std::cout << msg.toUtf8().constData() << std::endl;
            break;
        case QtWarningMsg:
            if (msg.size() > 0)
                std::cerr << HL_ERROR_PREFIX << msg.toUtf8().constData() << std::endl;
            else
                std::cerr << std::endl;
            break;
        case QtCriticalMsg:
            if (msg.size() > 0)
                std::cerr << HL_ERROR_PREFIX << msg.toUtf8().constData() << std::endl;
            else
                std::cerr << std::endl;
            break;
        case QtFatalMsg:
            std::cerr << HL_ERROR_PREFIX << "FATAL: " << msg.toUtf8().constData() << std::endl;
            std::cerr << "From: " << context.function << std::endl;
            std::cerr << "In: " << context.file << " at line " << context.line << std::endl;
            exit(1);
            break;
    }
}

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