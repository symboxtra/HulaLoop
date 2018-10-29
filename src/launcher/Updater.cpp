#include "Updater.h"

#include <QDataStream>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QProcess>

Updater::Updater(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    reply = nullptr;
    file = nullptr;

    updateHostUrl = "";
    updateAvailable = false;

    downloadHostUrl = "";
    numBytesDownloaded = 0;
    downloadSize = 0;
    downloaded = false;
}

void Updater::setUpdateHost(QString updateHostUrl)
{
    this->updateHostUrl = updateHostUrl;
}

bool Updater::foundUpdate()
{
    return updateAvailable;
}

qint64 Updater::getDownloadSize()
{
    return downloadSize;
}

qint64 Updater::getNumBytesDownloaded()
{
    return numBytesDownloaded;
}

bool Updater::finishedDownload()
{
    return downloaded;
}

void Updater::checkForUpdate()
{

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(updateQueryFinished(QNetworkReply *)));
    reply = manager->get(QNetworkRequest(QUrl(updateHostUrl)));

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

}

void Updater::downloadUpdate()
{

    QNetworkRequest req(downloadHostUrl);
    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    reply = manager->get(req);

    QString fileName = QFileInfo(reply->url().path()).fileName();
    file = new QFile(QDir::tempPath() + "/" + fileName);
    file->open(QIODevice::WriteOnly);

    connect(reply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateDownloadProgress(qint64, qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));

}

void Updater::updateQueryFinished(QNetworkReply *reply)
{

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if(doc.isNull())
        return;

    QJsonObject rootObj = doc.object();
    if(rootObj.isEmpty())
        return;

    QString tagName = rootObj["tag_name"].toString();
    // tagName.remove(0, 1); // Remove the 'v' before the version string

    QStringList versionParts = tagName.split('.', QString::SkipEmptyParts);

    if(versionParts.size() >= 3)
    {

        if(versionParts[0] > HL_VERSION_MAJOR)
            updateAvailable = true;
        if(versionParts[1] > HL_VERSION_MINOR)
            updateAvailable = true;
        if(versionParts[2] > HL_VERSION_REV)
            updateAvailable = true;

    }
    else
        return;

    if(updateAvailable)
    {

        QJsonArray assets = rootObj["assets"].toArray();
        for(int i = 0; i < assets.size(); i++)
        {

            QJsonObject obj = assets[i].toObject();
            QString assetName = obj["name"].toString();
            bool ok = false;

            if(assetName.contains("core") || assetName.contains(HL_PACKAGE_TYPE))
            {

                downloadHostUrl = obj["browser_download_url"].toString();
                downloadSize = obj["size"].toVariant().toULongLong(&ok);

                if(!ok)
                {
                    downloadHostUrl = "";
                    downloadSize = 0L;
                    return;
                }
                break;
            }

        }

    }
    reply = nullptr;
}

void Updater::downloadReadyRead()
{

    numBytesDownloaded = reply->bytesAvailable();
    emit bytesDownloaded();

    QDataStream out(file);
    out << reply->read(numBytesDownloaded);

}

void Updater::updateDownloadProgress(qint64 read, qint64 total)
{

}

void Updater::downloadFinished()
{

    file->flush();
    file->close();
    reply->deleteLater();

}

void Updater::startHulaLoopApp()
{

    QProcess proc;
    QString procName = QCoreApplication::applicationDirPath() + "/hulaloop";

    proc.setProgram(procName);
    if(proc.startDetached())
        exit(0);

}

Updater::~Updater()
{
    delete manager;
}