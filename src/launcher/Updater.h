#ifndef UPDATER_H
#define UPDATER_H

#include <HulaVersion.h>

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class Updater : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString updateHostUrl WRITE setUpdateHost)
    Q_PROPERTY(qint64 numBytesDownloaded READ getNumBytesDownloaded)

    private:
        QNetworkAccessManager *manager;
        QNetworkReply *reply;
        QFile *file;

        QString updateHostUrl;
        bool updateAvailable;

        QString downloadHostUrl;
        QString downloadFileName;
        qint64 numBytesDownloaded;
        qint64 downloadSize;
        bool downloaded;

    public:
        explicit Updater(QObject *parent = nullptr);
        ~Updater();

        Q_INVOKABLE void setUpdateHost(QString);
        Q_INVOKABLE bool foundUpdate();
        Q_INVOKABLE qint64 getDownloadSize();
        Q_INVOKABLE qint64 getNumBytesDownloaded();
        Q_INVOKABLE bool finishedDownload();

        Q_INVOKABLE void checkForUpdate();
        Q_INVOKABLE void downloadUpdate();

        Q_INVOKABLE void startHulaLoopInstaller();
        Q_INVOKABLE void startHulaLoopApp();

    public slots:
        void updateQueryFinished(QNetworkReply *);
        void downloadReadyRead();
        void downloadFinished();

    signals:
        void bytesDownloaded();

};

#endif // UPDATER_H