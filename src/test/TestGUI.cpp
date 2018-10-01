#include <gtest/gtest.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlProperty>
#include <QTimer>

#include "qmlbridge.h"

class TestGUI : public ::testing::Test {

    protected:
        QGuiApplication *app = nullptr;
        QQmlApplicationEngine *engine = nullptr;

        virtual void SetUp()
        {

            int argc = 0;
            char **argv = nullptr;

            app = new QGuiApplication(argc, argv);

            qmlRegisterType<QMLBridge>("hulaloop.qmlbridge", 1, 0, "QMLBridge");

            engine = new QQmlApplicationEngine();
            engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

        }

        virtual void TearDown()
        {

            app->exit();
            delete engine;
            delete app;

        }

        bool getStateAfterClick(QString objName, QString state)
        {

            QObject *btn = engine->rootObjects()[0]->findChild<QObject *>(objName);
            QObject *label = engine->rootObjects()[0]->findChild<QObject *>(QString::fromStdString("transportState"));
            QVariant property = NULL;

            if(btn && label)
            {

                QMetaObject::invokeMethod(btn, "clicked");
                property = QQmlProperty::read(label, QString::fromStdString("text"));

                if(property != NULL)
                    return property.toString() == state;

            }
            return false;
        }

};

TEST_F(TestGUI, init)
{

    ASSERT_TRUE(app != nullptr);
    ASSERT_TRUE(engine != nullptr);

    QTimer::singleShot(300, app, &QGuiApplication::quit);
    ASSERT_EQ(app->exec(), 0);

}

TEST_F(TestGUI, recordButton)
{
    ASSERT_TRUE(getStateAfterClick("recordBtn", "Recording"));
}

TEST_F(TestGUI, stopButton)
{
    ASSERT_TRUE(getStateAfterClick("stopBtn", "Stopped"));
}

TEST_F(TestGUI, playButton)
{
    ASSERT_TRUE(getStateAfterClick("playBtn", "Playing"));
}

TEST_F(TestGUI, pauseButton)
{
    ASSERT_TRUE(getStateAfterClick("pauseBtn", "Paused"));
}