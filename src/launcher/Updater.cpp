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

#include <QDebug>

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

    errorMsg = "";
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
QString Updater::getUpdateHost()
{
    return updateHostUrl;
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

    QList<int> versionParts;
    QStringList tagSegments = tagName.split('.', QString::SkipEmptyParts);

    if (tagSegments.size() < 3)
    {
        return QList<int>({-1, -1, -1});
    }

    for (int i = 0; i < tagSegments.size(); i++)
    {

        QString segment = tagSegments.at(i);
        segment.replace(QRegularExpression("[^0-9]"), "");

        bool ok = false;
        int ver = segment.toInt(&ok);

        (ok) ? versionParts.append(ver) : versionParts.append(-1);

    }
    return versionParts;
}

/**
 * Method to check the descriptive error message.
 *
 * @return QString - The error in human-readable format
 */
QString Updater::getErrorMessage()
{
    return errorMsg;
}

/**
 * Creates the network request using the updateHostUrl.
 * This method blocks until the finished signal is caught.
 *
 * @return int - 1 if found, 0 if not found, -1 if an error occurred
 */
int Updater::checkForUpdate()
{

    int updateAvailable = 0;
    reply = manager->get(QNetworkRequest(QUrl(updateHostUrl)));

    connect(reply, &QNetworkReply::sslErrors, [&](QList<QSslError> sslErr)
    {
        reply->deleteLater();
        updateAvailable = -1;

        errorMsg = "Updater: " + sslErr.at(0).errorString() + "!";
    });

    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), [&](QNetworkReply::NetworkError code)
    {
        reply->deleteLater();
        updateAvailable = -1;

        errorMsg = "Updater: " + reply->errorString() + "!";
    });

    connect(reply, &QNetworkReply::finished, [&]
    {

        if(reply->error())
            return;

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (doc.isNull())
            updateAvailable = 0;

        QJsonObject rootObj = doc.object();
        if (rootObj.isEmpty())
            updateAvailable = 0;

        QList<int> versionParts = parseTagName(rootObj["tag_name"].toString());
        if (versionParts.at(0) > HL_VERSION_MAJOR)
        {
            updateAvailable = 1;
        }
        else if (versionParts.at(1) > HL_VERSION_MINOR)
        {
            updateAvailable = 1;
        }
        else if (versionParts.at(2) > HL_VERSION_REV)
        {
            updateAvailable = 1;
        }

        // Found an update, check the assets of the release
        if (updateAvailable)
        {

            QJsonArray assets = rootObj["assets"].toArray();
            for (int i = 0; i < assets.size(); i++)
            {

                QJsonObject obj = assets[i].toObject();
                QString assetName = obj["name"].toString();
                bool ok = false;

                if (assetName.contains(".tar") || assetName.contains(HL_PACKAGE_TYPE))
                {

                    downloadHostUrl = obj["browser_download_url"].toString();
                    downloadFileName = obj["name"].toString();
                    downloadSize = obj["size"].toVariant().toULongLong(&ok);

                    if (!ok)
                    {
                        downloadHostUrl = "";
                        downloadFileName = "";
                        downloadSize = 0L;

                        updateAvailable = -1;
                        errorMsg = "Updater: Error obtaining download size!";
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
 * @return int - True if the download finished successfully, false otherwise
 */
int Updater::downloadUpdate()
{

    int finishedDownload = 0;

    QNetworkRequest req(downloadHostUrl);
    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    reply = manager->get(req);

    QString fileName = QFileInfo(reply->url().path()).fileName();
    QFile file(QDir::tempPath() + "/" + fileName);
    file.open(QIODevice::WriteOnly);

    connect(reply, &QNetworkReply::sslErrors, [&](QList<QSslError> sslErr)
    {
        // throw exception
        file.close();
        reply->deleteLater();
        finishedDownload = -1;

        errorMsg = "Downloader: " + sslErr.at(0).errorString() + "!";
    });

    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), [&](QNetworkReply::NetworkError code)
    {
        // throw exception
        file.close();
        reply->deleteLater();
        finishedDownload = -1;

        errorMsg = "Downloader: " + reply->errorString() + "!";
    });

    connect(reply, &QNetworkReply::readyRead, [&]
    {

        if(reply->error())
            return;

        numBytesDownloaded = reply->bytesAvailable();
        emit bytesDownloaded();
        file.write(reply->readAll());

    });

    connect(reply, &QNetworkReply::finished, [&]
    {

        if(reply->error())
            return;

        file.flush();
        file.close();
        reply->deleteLater();

        finishedDownload = 1;

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
    if (proc.startDetached())
    {
        exit(0);
    }
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
    if (proc.startDetached())
    {
        exit(0);
    }
}

/**
 * Destroys the Updater object.
 */
Updater::~Updater()
{
    delete manager;
}
