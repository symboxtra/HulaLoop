#ifndef QMLBRIDGE_H
#define QMLBRIDGE_H

#include <QObject>
#include <QString>

#include "../control/transport.h"

class QMLBridge : public QObject
{
	Q_OBJECT

	private:
		Transport *transport;

	public:
		explicit QMLBridge(QObject *parent = nullptr);

		Q_INVOKABLE QString getTransportState() const;
		Q_INVOKABLE void record();
		Q_INVOKABLE void stop();
		Q_INVOKABLE void play();
		Q_INVOKABLE void pause();

	signals:
		void stateChanged();
};

#endif // QMLBRIDGE_H