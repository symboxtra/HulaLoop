#ifndef HL_UPDATER_H
#define HL_UPDATER_H

#include <HulaVersion.h>

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

namespace hula
{
    class Updater : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString updateHostUrl WRITE setUpdateHost)
        Q_PROPERTY(qint64 downloadSize READ getDownloadSize)
        Q_PROPERTY(qint64 numBytesDownloaded READ getNumBytesDownloaded)

        private:
            QNetworkAccessManager *manager;
            QNetworkReply *reply;
            QFile *file;

            QString updateHostUrl;
            QString downloadHostUrl;
            QString downloadFileName;

            qint64 numBytesDownloaded;
            qint64 downloadSize;

        public:
            explicit Updater(QObject *parent = nullptr);
            ~Updater();

            void setUpdateHost(QString);

            QString getDownloadFileName();
            qint64 getNumBytesDownloaded();
            qint64 getDownloadSize();

            QList<int> parseTagName(const QString &);

            Q_INVOKABLE bool checkForUpdate();
            Q_INVOKABLE bool downloadUpdate();

            Q_INVOKABLE void startHulaLoopInstaller();
            Q_INVOKABLE void startHulaLoopApp();

        signals:
            void bytesDownloaded();

    };
}

#endif // HL_UPDATER_H