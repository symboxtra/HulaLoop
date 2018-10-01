#include "qmlbridge.h"

/**
 * Construct a QMLBridge.
 */
QMLBridge::QMLBridge(QObject *parent) : QObject(parent) {}

QString QMLBridge::getState()
{
	return state;
}

void QMLBridge::setState(const QString &state)
{

	if(this->state == state)
		return;

	this->state = state;
	emit stateChanged();

}