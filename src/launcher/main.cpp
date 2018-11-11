#include <QApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QQmlApplicationEngine>
#include <QQmlDebuggingEnabler>
#include <QQuickStyle>

#include "Updater.h"

using namespace hula;

int main(int argc, char *argv[])
{
    QQmlDebuggingEnabler debug;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterType<Updater>("hulaloop.updater", 1, 0, "Updater");

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    if (argc > 1)
    {
        QString arg(argv[1]);
        if (arg == "silent")
        {
            Updater updater;
            updater.setUpdateHost("https://api.github.com/repos/jmcker/HulaLoop/releases/latest");

            updater.checkForUpdate();

            QMessageBox msgBox;

            msgBox.setWindowTitle("HulaLoop Updater");
            msgBox.setIconPixmap(QPixmap(":/res/hulaloop-logo.png"));

            if (updater.foundUpdate())
            {
                msgBox.setText("A newer version has been detected!\n\nClick Ok to update!");
                msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);

                if (msgBox.exec() == QMessageBox::Ok)
                {
                    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
                    if (engine.rootObjects().isEmpty())
                    {
                        return -1;
                    }
                    return app.exec();
                }
                else
                {
                    QApplication::quit();
                }
            }
            else
            {
                msgBox.setText("All systems are up-to-date!");
                msgBox.setStandardButtons(QMessageBox::Ok);

                msgBox.exec();
            }
        }
        else if ("debug")
        {
            Updater updater;
            updater.setUpdateHost("https://api.github.com/repos/jmcker/HulaLoop/releases/latest");

            updater.checkForUpdate();
            updater.setUpdateAvailable(false);

            QMessageBox msgBox;

            msgBox.setWindowTitle("HulaLoop Updater");
            msgBox.setIconPixmap(QPixmap(":/res/hulaloop-logo.png"));

            if (updater.foundUpdate())
            {
                msgBox.setText("A newer version has been detected!\n\nClick Ok to update!");
                msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);

                if (msgBox.exec() == QMessageBox::Ok)
                {
                    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
                    if (engine.rootObjects().isEmpty())
                    {
                        return -1;
                    }
                    return app.exec();
                }
                else
                {
                    QApplication::quit();
                }
            }
            else
            {
                msgBox.setText("All systems are up-to-date!");
                msgBox.setStandardButtons(QMessageBox::Ok);

                msgBox.exec();
                updater.startHulaLoopApp();
            }
        }
    }
    else
    {
        engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
        if (engine.rootObjects().isEmpty())
        {
            return -1;
        }
        return app.exec();
    }
    return 0;
}