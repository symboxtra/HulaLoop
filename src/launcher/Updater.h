#ifndef HL_UPDATER_H
#define HL_UPDATER_H

#include <HulaVersion.h>

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

namespace hula
{
    class Updater : public QObject {
            Q_OBJECT

            /**
             * Exposes the updateHostUrl property to QML.
             */
            Q_PROPERTY(QString updateHostUrl READ getUpdateHost WRITE setUpdateHost)

            /**
             * Exposes the downloadSize property to QML.
             */
            Q_PROPERTY(qint64 downloadSize READ getDownloadSize)

            /**
             * Exposes the numBytesDownloaded property to QML.
             */
            Q_PROPERTY(qint64 numBytesDownloaded READ getNumBytesDownloaded)

        private:
            QNetworkAccessManager *manager;
            QNetworkReply *reply;

            QString errorMsg;
            QString updateHostUrl;
            QString downloadHostUrl;
            QString downloadFileName;

            qint64 numBytesDownloaded;
            qint64 downloadSize;

        public:
            explicit Updater(QObject *parent = nullptr);
            ~Updater();

            void setUpdateHost(QString);
            QString getUpdateHost();

            QString getDownloadFileName();
            qint64 getNumBytesDownloaded();
            qint64 getDownloadSize();

            QList<int> parseTagName(const QString &);

            Q_INVOKABLE QString getErrorMessage();
            Q_INVOKABLE int checkForUpdate();
            Q_INVOKABLE int downloadUpdate();

            Q_INVOKABLE void startHulaLoopInstaller();
            Q_INVOKABLE void startHulaLoopApp();

        signals:
            /**
             * A signal emitted when QNetworkReply's readyRead signal is triggered.
             *
             * This method informs the QML side of how many bytes were read by the
             * network request and is used to update the progress bar accordingly.
             */
            void bytesDownloaded();
    };
}

#endif // HL_UPDATER_H