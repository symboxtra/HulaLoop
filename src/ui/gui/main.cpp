#include <QFontDatabase>
#include <QApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlDebuggingEnabler>
#include <QQuickStyle>

#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

#include <QtDebug>

#include "QMLBridge.h"

int main(int argc, char *argv[])
{

    QQmlDebuggingEnabler debug;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/res/hulaloop-logo.png"));

    qmlRegisterType<QMLBridge>("hulaloop.qmlbridge", 1, 0, "QMLBridge");

    QFontDatabase::addApplicationFont(":/fonts/materialdesignicons-webfont.ttf");
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    QObject *root = engine.rootObjects().at(0);

    QAction *minimizeAction = new QAction(QObject::tr("Mi&nimize"), root);
    root->connect(minimizeAction, SIGNAL(triggered()), root, SLOT(hide()));
    QAction *maximizeAction = new QAction(QObject::tr("Ma&ximize"), root);
    root->connect(maximizeAction, SIGNAL(triggered()), root, SLOT(showMaximized()));
    QAction *restoreAction = new QAction(QObject::tr("&Restore"), root);
    root->connect(restoreAction, SIGNAL(triggered()), root, SLOT(showNormal()));
    QAction *quitAction = new QAction(QObject::tr("&Quit"), root);
    root->connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu *trayIconMenu = new QMenu();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(root);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/res/hulaloop-logo.svg"));
    trayIcon->show();

    trayIcon->showMessage("Test", "Message", QIcon(":/res/hulaloop-logo.png"), 10000);

    return app.exec();
}
