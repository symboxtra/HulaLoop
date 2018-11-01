#include "Updater.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QDir>
#include <QEventLoop>
#include <QFile>
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
    downloadFileName = "";
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

    // updateAvailable = false;

    if(updateAvailable)
    {

        QJsonArray assets = rootObj["assets"].toArray();
        for(int i = 0; i < assets.size(); i++)
        {

            QJsonObject obj = assets[i].toObject();
            QString assetName = obj["name"].toString();
            bool ok = false;

            if(assetName.contains(".tar") || assetName.contains(HL_PACKAGE_TYPE))
            {

                downloadHostUrl = obj["browser_download_url"].toString();
                downloadFileName = obj["name"].toString();
                downloadSize = obj["size"].toVariant().toULongLong(&ok);

                if(!ok)
                {
                    downloadHostUrl = "";
                    downloadFileName = "";
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

    file->write(reply->readAll());

}

void Updater::downloadFinished()
{

    file->flush();
    file->close();
    reply->deleteLater();

    downloaded = true;

}

void Updater::startHulaLoopInstaller()
{

    QProcess proc;
    QString procName = QDir::tempPath() + "/HulaLoop/" + downloadFileName;

    proc.setProgram(procName);
    if(proc.startDetached())
        exit(0);

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