#include "qmlbridge.h"

#include "../control/transport.h"

QMLBridge::QMLBridge(QObject *parent) : QObject(parent)
{
    transport = new Transport;
}

QString QMLBridge::getTransportState() const
{
    return QString::fromStdString(transport->stateToStr(transport->getState()));
}

void QMLBridge::record()
{
    transport->record();
    emit stateChanged();
}

void QMLBridge::stop()
{
    transport->stop();
    emit stateChanged();
}

void QMLBridge::play()
{
    transport->play();
    emit stateChanged();
}

void QMLBridge::pause()
{
    transport->pause();
    emit stateChanged();
}