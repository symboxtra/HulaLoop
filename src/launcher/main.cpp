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

            QMessageBox msgBox;
            msgBox.setWindowTitle(Updater::tr("HulaLoop Updater"));
            msgBox.setIconPixmap(QPixmap(":/res/hulaloop-logo.png"));

            int ret = updater.checkForUpdate();

            if (ret == 1)
            {

                msgBox.setText(Updater::tr("A newer version has been detected!") + "\n\n" + Updater::tr("Click Ok to update!"));
                msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);

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
            else if (ret == 0)
            {
                msgBox.setText(Updater::tr("All systems are up-to-date!"));
                msgBox.setStandardButtons(QMessageBox::Ok);

                msgBox.exec();
            }
            else
            {
                msgBox.setText(updater.getErrorMessage());
                msgBox.setStandardButtons(QMessageBox::Ok);

                msgBox.exec();
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