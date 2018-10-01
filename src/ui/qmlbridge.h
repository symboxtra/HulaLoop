#ifndef QMLBRIDGE_H
#define QMLBRIDGE_H

#include <QObject>
#include <QString>

#include "../control/transport.h"

class QMLBridge : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString state READ getState WRITE setState NOTIFY stateChanged)

	private:
		QString state;

	public:
		explicit QMLBridge(QObject *parent = nullptr);

		QString getState();
		void setState(const QString &state);

	signals:
		void stateChanged();
};

#endif // QMLBRIDGE_H