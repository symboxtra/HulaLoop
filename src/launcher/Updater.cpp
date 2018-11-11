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
#include <QRegularExpression>

using namespace hula;

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
    downloadHostUrl = "";
    downloadFileName = "";

    numBytesDownloaded = 0;
    downloadSize = 0;
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
 * Method to check the downloaded file name.
 *
 * @return QString - The name of the downloaded file
 */
QString Updater::getDownloadFileName()
{
    return downloadFileName;
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
 * Method to check the size of the download.
 *
 * @return qint64 - The size of the download file
 */
qint64 Updater::getDownloadSize()
{
    return downloadSize;
}

/**
 * A method to parse the tag name returned from the GitHub Releases API.
 *
 * @param tagName - QString containing the GitHub Releases tag name
 * @return QList<int> - A QList holding the version segments (major, minor, revision, build)
 */
QList<int> Updater::parseTagName(const QString &tagName)
{

    QList<int> versionParts({-1, -1, -1, -1});
    QStringList tagSegments = tagName.split('.', QString::SkipEmptyParts);

    if(tagSegments.size() < 3)
        return versionParts;

    for(int i = 0; i < tagSegments.size(); i++)
    {

        QString segment = tagSegments.at(i);
        segment.replace(QRegularExpression("[^0-9]"), "");

        bool ok = false;
        int ver = segment.toInt(&ok);

        (ok) ? versionParts[i] = ver : versionParts[i] = -1;

    }
    return versionParts;
}

/**
 * Creates the network request using the updateHostUrl.
 * This method blocks until the finished signal is caught.
 *
 * @return bool - True if an update was found, false otherwise
 */
bool Updater::checkForUpdate()
{

    bool updateAvailable = false;
    reply = manager->get(QNetworkRequest(QUrl(updateHostUrl)));

    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), [=](QNetworkReply::NetworkError code) {
        // throw exception
        reply->deleteLater();
        return false;
    });

    connect(reply, &QNetworkReply::sslErrors, [=] {
        // throw exception
        reply->deleteLater();
        return false;
    });

    connect(reply, &QNetworkReply::finished, [&] {

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if(doc.isNull())
            return false;

        QJsonObject rootObj = doc.object();
        if(rootObj.isEmpty())
            return false;

        QList<int> versionParts = parseTagName(rootObj["tag_name"].toString());
        if(versionParts.size() == 4)
        {

            if(versionParts.at(0) > HL_VERSION_MAJOR)
                updateAvailable = true;
            else if(versionParts.at(1) > HL_VERSION_MINOR)
                updateAvailable = true;
            else if(versionParts.at(2) > HL_VERSION_REV)
                updateAvailable = true;
            else if(versionParts.at(3) > HL_VERSION_BUILD)
                updateAvailable = true;

        }
        else
            return false;

        // Found an update, check the assets of the release
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
                        // TODO: Handle exception
                        downloadHostUrl = "";
                        downloadFileName = "";
                        downloadSize = 0L;
                        return false;
                    }
                    break;
                }

            }

        }

    });

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return updateAvailable;
}

/**
 * Creates the network request using the downloadHostUrl.
 * This method writes incrementally to a file in the system's temporary
 * directory and blocks until the finished signal is caught.
 *
 * @return bool - True if the download finished successfully, false otherwise
 */
bool Updater::downloadUpdate()
{

    bool finishedDownload = false;

    QNetworkRequest req(downloadHostUrl);
    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    reply = manager->get(req);

    QString fileName = QFileInfo(reply->url().path()).fileName();
    file = new QFile(QDir::tempPath() + "/" + fileName);
    file->open(QIODevice::WriteOnly);

    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), [=](QNetworkReply::NetworkError code) {
        // throw exception
        file->close();
        delete file;
        reply->deleteLater();
        return false;
    });

    connect(reply, &QNetworkReply::sslErrors, [=] {
        // throw exception
        file->close();
        delete file;
        reply->deleteLater();
        return false;
    });

    connect(reply, &QNetworkReply::readyRead, [&] {

        numBytesDownloaded = reply->bytesAvailable();
        emit bytesDownloaded();
        file->write(reply->readAll());

    });

    connect(reply, &QNetworkReply::finished, [&] {

        file->flush();
        file->close();

        delete file;
        reply->deleteLater();

        finishedDownload = true;

    });

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return finishedDownload;
}

/**
 * Opens the HulaLoop installer application. This method is called only if
 * an update was found.
 */
void Updater::startHulaLoopInstaller()
{

    QProcess proc;
    QString procName = QDir::tempPath() + "/" + downloadFileName;

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