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

/**
 * Construct a new instance of the Updater class.
 *
 * @param parent Object which Updater should be child of.
 */
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

/**
 * Sets the update host for the Updater
 *
 * @param updateHostUrl - String host URL
 */
void Updater::setUpdateHost(QString updateHostUrl)
{
    this->updateHostUrl = updateHostUrl;
}

/**
 * Method to check if an update was found.
 *
 * @return bool - If an update was found
 */
bool Updater::foundUpdate()
{
    return updateAvailable;
}

/**
 * Method to check the size of the download.
 *
 * @return qint64 - The size of the download file
 */
qint64 Updater::getDownloadSize()
{
    return downloadSize;
}

/**
 * Method to check how many bytes have been downloaded.
 *
 * @return qint64 - The number of bytes downloaded
 */
qint64 Updater::getNumBytesDownloaded()
{
    return numBytesDownloaded;
}

/**
 * Method to check if the download has finished.
 *
 * @return bool - If the download has finished
 */
bool Updater::finishedDownload()
{
    return downloaded;
}

/**
 * Creates the network request using the updateHostUrl.
 * This method blocks until the finished signal is caught.
 */
void Updater::checkForUpdate()
{

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(updateQueryFinished(QNetworkReply *)));
    reply = manager->get(QNetworkRequest(QUrl(updateHostUrl)));

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

}

/**
 * Creates the network request using the downloadHostUrl.
 * This method writes incrementally to a file in the system's
 * temporary directory.
 */
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

/**
 * Parses the network reply based off the GitHub Releases API to check
 * if an update is available.
 *
 * @param reply - NetworkReply that is received by the update check
 */
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

/**
 * This method catches the readyRead signal and writes the number of read
 * bytes to the file.
 */
void Updater::downloadReadyRead()
{

    numBytesDownloaded = reply->bytesAvailable();
    emit bytesDownloaded();

    file->write(reply->readAll());

}

/**
 * This method catches the finished signal and runs cleanup code to handle
 * the open file and NetworkReply.
 */
void Updater::downloadFinished()
{

    file->flush();
    file->close();
    reply->deleteLater();

    downloaded = true;

}

/**
 * Opens the HulaLoop installer application. This method is called only if
 * an update was found.
 */
void Updater::startHulaLoopInstaller()
{

    QProcess proc;
    QString procName = QDir::tempPath() + "/HulaLoop/" + downloadFileName;

    proc.setProgram(procName);
    if(proc.startDetached())
        exit(0);

}

/**
 * Opens the HulaLoop application. This method is called only if
 * no update was found.
 */
void Updater::startHulaLoopApp()
{

    QProcess proc;
    QString procName = QCoreApplication::applicationDirPath() + "/hulaloop";

    proc.setProgram(procName);
    if(proc.startDetached())
        exit(0);

}

/**
 * Destroys the Updater object.
 */
Updater::~Updater()
{
    delete manager;
}