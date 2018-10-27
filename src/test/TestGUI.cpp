#include <gtest/gtest.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlProperty>
#include <QQuickItem>
#include <QTimer>

#include "QMLBridge.h"

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

        QString getTransportState()
        {

            QObject *label = engine->rootObjects()[0]->findChild<QObject *>(QString::fromStdString("transportState"));

            if (label)
            {

                QVariant property = QQmlProperty::read(label, QString::fromStdString("text"));
                if (!property.isNull())
                {
                    return property.toString();
                }

            }
            return QString();
        }

        bool isVisible(QString objName)
        {

            QObject *obj = engine->rootObjects()[0]->findChild<QObject *>(objName);

            if (obj)
            {
                return obj->property("visible").toBool();
            }

            return false;
        }

        void clickButton(QString objName)
        {

            QObject *btn = engine->rootObjects()[0]->findChild<QObject *>(objName);

            if (btn)
            {
                QMetaObject::invokeMethod(btn, "clicked");
            }

        }

        void startTimers()
        {

            QObject *timer = engine->rootObjects()[0]->findChild<QObject *>("countDownTimer");
            QObject *delayIn = engine->rootObjects()[0]->findChild<QObject *>("delayInput");
            QObject *recordIn = engine->rootObjects()[0]->findChild<QObject *>("recordTimeInput");

            if (timer && delayIn && recordIn)
            {

                delayIn->setProperty("text", "00:00:01");
                recordIn->setProperty("text", "00:00:01");
                timer->setProperty("running", "true");

            }

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
    clickButton("recordBtn");
    ASSERT_TRUE(getTransportState() == "Recording");
}

TEST_F(TestGUI, stopButton)
{
    clickButton("stopBtn");
    ASSERT_TRUE(getTransportState() == "Stopped");
}

TEST_F(TestGUI, playButton)
{
    clickButton("playBtn");
    ASSERT_TRUE(getTransportState() == "Playing");
}

TEST_F(TestGUI, pauseButton)
{
    clickButton("pauseBtn");
    ASSERT_TRUE(getTransportState() == "Paused");
}

TEST_F(TestGUI, timerButton)
{
    clickButton("timerBtn");
    ASSERT_TRUE(isVisible("timerPopup"));
}

TEST_F(TestGUI, exportButton)
{
    clickButton("exportBtn");
    ASSERT_TRUE(isVisible("saveDialog"));
}

TEST_F(TestGUI, timers)
{
    startTimers();

    //Set initial timer to 3000 ms to account for startup time
    QTimer::singleShot(3000, [ = ]
    {
        ASSERT_TRUE(getTransportState() == "Recording");
    });

    QTimer::singleShot(4000, [ = ]
    {
        ASSERT_TRUE(getTransportState() == "Stopped");
    });
}
