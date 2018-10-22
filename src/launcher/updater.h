#ifndef UPDATER_H
#define UPDATER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class Updater : public QObject
{
    Q_OBJECT

    private:
        QNetworkAccessManager *manager;
        QUrl updateHostUrl;
        bool updateFound;

    public:
        explicit Updater(QObject *parent = nullptr);
        ~Updater();

        void setUpdateHost(QString updateHost);
        void checkForUpdate();

    public slots:
        void updateCheckFinished(QNetworkReply *);
};

#endif // UPDATER_H