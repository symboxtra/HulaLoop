#include <gtest/gtest.h>

#include <QApplication>
#include <QQmlApplicationEngine>

#include <QQmlProperty>
#include <QQuickItem>
#include <QTimer>
#include <iostream>

#include "QMLBridge.h"
#include "SystemTrayIcon.h"

using namespace hula;

class TestGUI : public ::testing::Test {

    protected:
        QApplication *app = nullptr;
        QQmlApplicationEngine *engine = nullptr;

        virtual void SetUp()
        {
            int argc = 0;
            char **argv = nullptr;

            app = new QApplication(argc, argv);

            qmlRegisterType<QMLBridge>("hulaloop.qmlbridge", 1, 0, "QMLBridge");
            qmlRegisterType<SystemTrayIcon>("hulaloop.systrayicon", 1, 0, "SystemTrayIcon");

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
                else
                {
                    ADD_FAILURE();
                }

            }
            else
            {
                ADD_FAILURE();
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
            else
            {
                ADD_FAILURE();
            }

            return false;
        }

        bool isEnabled(QString objName)
        {
            QObject *obj = engine->rootObjects()[0]->findChild<QObject *>(objName);

            if (obj)
            {
                return obj->property("enabled").toBool();
            }
            else
            {
                ADD_FAILURE();
            }

            return false;
        }

        bool isButtonPlay()
        {
            QObject *obj = engine->rootObjects()[0]->findChild<QObject *>("play_icon");

            if (obj)
            {
                return QString::compare(obj->property("color").toString(), "#ffffff");
            }
            else
            {
                ADD_FAILURE();
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
            else
            {
                ADD_FAILURE();
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
            else
            {
                ADD_FAILURE();
            }
        }

};

TEST_F(TestGUI, init)
{

    ASSERT_TRUE(app != nullptr);
    ASSERT_TRUE(engine != nullptr);

    QTimer::singleShot(300, app, &QApplication::quit);
    ASSERT_EQ(app->exec(), 0);

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

TEST_F(TestGUI, timerButton)
{
    clickButton("timerBtn");
    ASSERT_TRUE(isVisible("timerPopup"));
}


/**
 * UI State Machine Tests
 *
 * KEY :
 *      R  - Record
 *      S  - Stop
 *      PL - Playback
 *      PA - Pause
 *      E  - Export
 */

/**
 * Simulate button presses with different combination paths
 *
 * EXPECTED:
 *      The buttons respond based on the following path:
 *          R -> S -> E
 */
TEST_F(TestGUI, ui_state_machine_1)
{
    // Click Record button
    clickButton("recordBtn");
    ASSERT_EQ(getTransportState(), "Recording");

    EXPECT_FALSE(isEnabled("recordBtn"));
    EXPECT_TRUE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_FALSE(isButtonPlay());

    // Click Stop button
    clickButton("stopBtn");
    ASSERT_EQ(getTransportState(), "Stopped");

    EXPECT_FALSE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_TRUE(isEnabled("exportBtn"));

    ASSERT_TRUE(isButtonPlay());

    // Click Export button
    // TODO: Determine expected behavior
}

/**
 * Simulate button presses with different combination paths
 *
 * EXPECTED:
 *      The buttons respond based on the following path:
 *          R -> S -> PL -> PA
 */
TEST_F(TestGUI, ui_state_machine_2)
{
    // Click Record button
    clickButton("recordBtn");
    ASSERT_EQ(getTransportState(), "Recording");

    EXPECT_FALSE(isEnabled("recordBtn"));
    EXPECT_TRUE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_FALSE(isButtonPlay());

    // Click Stop button
    clickButton("stopBtn");
    ASSERT_EQ(getTransportState(), "Stopped");

    EXPECT_TRUE(isEnabled("recordBtn"));
    EXPECT_FALSE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_TRUE(isEnabled("exportBtn"));

    ASSERT_TRUE(isButtonPlay());

    // Click Play button
    clickButton("playpauseBtn");
    ASSERT_EQ(getTransportState(), "Playing");

    EXPECT_TRUE(isEnabled("recordBtn"));
    EXPECT_FALSE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_TRUE(isEnabled("exportBtn"));

    ASSERT_FALSE(isButtonPlay());

    // Click Pause button
    clickButton("playpauseBtn");
    ASSERT_EQ(getTransportState(), "Paused");

    EXPECT_TRUE(isEnabled("recordBtn"));
    EXPECT_FALSE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_TRUE(isEnabled("exportBtn"));

    ASSERT_TRUE(isButtonPlay());
}

/**
 * Simulate button presses with different combination paths
 *
 * EXPECTED:
 *      The buttons respond based on the following path:
 *          R -> PA -> S
 */
TEST_F(TestGUI, ui_state_machine_3)
{
    // Click Record button
    clickButton("recordBtn");
    ASSERT_EQ(getTransportState(), "Recording");

    EXPECT_FALSE(isEnabled("recordBtn"));
    EXPECT_TRUE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_FALSE(isButtonPlay());

    // Click Pause button
    clickButton("playpauseBtn");
    ASSERT_EQ(getTransportState(), "Paused");

    EXPECT_TRUE(isEnabled("recordBtn"));
    EXPECT_TRUE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_TRUE(isButtonPlay());

    // Click Stop button
    clickButton("stopBtn");
    ASSERT_EQ(getTransportState(), "Stopped");

    EXPECT_TRUE(isEnabled("recordBtn"));
    EXPECT_FALSE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_TRUE(isEnabled("exportBtn"));

    ASSERT_TRUE(isButtonPlay());
}

/**
 * Simulate button presses with different combination paths
 *
 * EXPECTED:
 *      The buttons respond based on the following path:
 *          R -> PA -> R
 */
TEST_F(TestGUI, ui_state_machine_4)
{
    // Click Record button
    clickButton("recordBtn");
    ASSERT_EQ(getTransportState(), "Recording");

    EXPECT_FALSE(isEnabled("recordBtn"));
    EXPECT_TRUE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_FALSE(isButtonPlay());

    // Click Pause button
    clickButton("playpauseBtn");
    ASSERT_EQ(getTransportState(), "Paused");

    EXPECT_TRUE(isEnabled("recordBtn"));
    EXPECT_TRUE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_TRUE(isButtonPlay());

    // Click Record button
    clickButton("recordBtn");
    ASSERT_EQ(getTransportState(), "Recording");

    EXPECT_FALSE(isEnabled("recordBtn"));
    EXPECT_TRUE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_FALSE(isButtonPlay());
}

/**
 * Simulate button presses with different combination paths
 *
 * EXPECTED:
 *      The buttons respond based on the following path:
 *          R -> PA -> PL
 */
TEST_F(TestGUI, ui_state_machine_5)
{
    // Click Record button
    clickButton("recordBtn");
    ASSERT_EQ(getTransportState(), "Recording");

    EXPECT_FALSE(isEnabled("recordBtn"));
    EXPECT_TRUE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_FALSE(isButtonPlay());

    // Click Pause button
    clickButton("playpauseBtn");
    ASSERT_EQ(getTransportState(), "Paused");

    EXPECT_TRUE(isEnabled("recordBtn"));
    EXPECT_TRUE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_TRUE(isButtonPlay());

    // Click Record button
    clickButton("playpauseBtn");
    ASSERT_EQ(getTransportState(), "Playing");

    EXPECT_FALSE(isEnabled("recordBtn"));
    EXPECT_FALSE(isEnabled("stopBtn"));
    EXPECT_TRUE(isEnabled("playpauseBtn"));
    EXPECT_FALSE(isEnabled("exportBtn"));

    ASSERT_FALSE(isButtonPlay());
}