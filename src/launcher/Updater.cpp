#include "Updater.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#define VERSION_MAJOR "4"
#define VERSION_MINOR "0"
#define VERSION_REVISION "1"

Updater::Updater(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    updateFound = false;
}

void Updater::setUpdateHost(QString updateHost)
{
    updateHostUrl = QUrl(updateHost);
}

void Updater::checkForUpdate()
{

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(updateCheckFinished(QNetworkReply *)));
    manager->get(QNetworkRequest(updateHostUrl));

}

void Updater::updateCheckFinished(QNetworkReply *reply)
{

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();

    QString tagName = obj["tag_name"].toString();
    tagName.remove(0, 1);

    QStringList verSegments = tagName.split('.');
    if(verSegments[0] > VERSION_MAJOR)
        updateFound = true;
    if(verSegments[1] > VERSION_MINOR)
        updateFound = true;
    if(verSegments[2] > VERSION_REVISION)
        updateFound = true;

    if(updateFound)
    {

        connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(downloadUpdate(QNetworkReply *)));

        QJsonValue assets = obj.value("assets");
        QJsonArray arr = assets.toArray();
        qDebug() << arr.at(1);
        manager->get(QNetworkRequest(arr.at(1).toString()));

    }

}

Updater::~Updater()
{
    delete manager;
}