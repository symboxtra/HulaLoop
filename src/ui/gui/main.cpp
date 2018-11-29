#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#ifndef NDEBUG
    #include <QQmlDebuggingEnabler>
#endif

#include "QMLBridge.h"
#include "SystemTrayIcon.h"

using namespace hula;

int main(int argc, char *argv[])
{
    #ifndef NDEBUG
    QQmlDebuggingEnabler debug;
    #endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/res/hulaloop-logo.png"));

    HulaSettings *settings = HulaSettings::getInstance();
    QTranslator *trans = settings->getTranslator();
    app.installTranslator(trans);

    qmlRegisterType<QMLBridge>("hulaloop.qmlbridge", 1, 0, "QMLBridge");
    qmlRegisterType<SystemTrayIcon>("hulaloop.systrayicon", 1, 0, "SystemTrayIcon");

    QFontDatabase::addApplicationFont(":/fonts/materialdesignicons-webfont.ttf");
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    // QAction *minimizeAction = new QAction(QObject::tr("Mi&nimize"), root);
    // root->connect(minimizeAction, SIGNAL(triggered()), root, SLOT(hide()));
    // QAction *maximizeAction = new QAction(QObject::tr("Ma&ximize"), root);
    // root->connect(maximizeAction, SIGNAL(triggered()), root, SLOT(showMaximized()));
    // QAction *restoreAction = new QAction(QObject::tr("&Restore"), root);
    // root->connect(restoreAction, SIGNAL(triggered()), root, SLOT(showNormal()));
    // QAction *quitAction = new QAction(QObject::tr("&Quit"), root);
    // root->connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    return app.exec();
}
