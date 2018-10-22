#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlDebuggingEnabler>
#include <QQuickStyle>
#include <QtDebug>

#include "Updater.h"

int main(int argc, char *argv[])
{

    QQmlDebuggingEnabler debug;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    Updater updater;
    updater.setUpdateHost("https://api.github.com/repos/segmentio/evergreen/releases/latest");
    updater.checkForUpdate();

    return app.exec();
}