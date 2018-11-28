#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#ifndef NDEBUG
#include <QQmlDebuggingEnabler>
#endif

#include "switch.h"
#include "style.h"

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
    QWidget container;
    QVBoxLayout mainLayout;
    container.setLayout(&mainLayout);

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

    /*Switch* switch1 = new Switch("SWITCH");
        mainLayout.addWidget(switch1);
        Switch* switch2 = new Switch("SWITCH");
        mainLayout.addWidget(switch2);
        switch2->setDisabled(true);
        Switch* switch3 = new Switch("SWITCH");
        mainLayout.addWidget(switch3);
        switch3->setLayoutDirection(Qt::RightToLeft);
        Switch* switch4 = new Switch("SWITCH");
        mainLayout.addWidget(switch4);
        switch4->setLayoutDirection(Qt::RightToLeft);
        switch4->setChecked(true);
        switch4->setDisabled(true);

        QButtonGroup bg;
        Switch* item1 = new Switch("ITEM1");
        Switch* item2 = new Switch("ITEM2");
        bg.addButton(item1);
        bg.addButton(item2);
        mainLayout.addWidget(item1);
        mainLayout.addWidget(item2);
        mainLayout.setMargin(100);

    container.show();*/
    return app.exec();
}
