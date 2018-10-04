#include "qmlbridge.h"

#include "../control/transport.h"

/**
 * Construct a new instance of the QMLBridge class.
 *
 * @param parent Object which QMLBridge should be child of.
 */
QMLBridge::QMLBridge(QObject *parent) : QObject(parent)
{
    transport = new Transport;
}

/**
 * Return the current state of the Transport instance as a QString.
 *
 * @return state State of Transport instance
 */
QString QMLBridge::getTransportState() const
{
    return QString::fromStdString(transport->stateToStr(transport->getState()));
}

/**
 * Trigger record in the Transport and update the UI state via signal.
 */
void QMLBridge::record()
{
    transport->record();
    emit stateChanged();
}

/**
 * Trigger stop in the Transport and update the UI state via signal.
 */
void QMLBridge::stop()
{
    transport->stop();
    emit stateChanged();
}

/**
 * Trigger playback in the Transport and update the UI state via signal.
 */
void QMLBridge::play()
{
    transport->play();
    emit stateChanged();
}

/**
 * Trigger pause in the Transport and update the UI state via signal.
 */
void QMLBridge::pause()
{
    transport->pause();
    emit stateChanged();
}