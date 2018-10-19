#include <gtest/gtest.h>
#include <hlaudio/hlaudio.h>
#include "TestCallback.h"

// Timeout code from: http://antonlipov.blogspot.com/2015/08/how-to-timeout-tests-in-gtest.html
#include <future>
#define TEST_TIMEOUT_BEGIN          std::promise<bool> promisedFinished; \
                                    auto futureResult = promisedFinished.get_future(); \
                                    std::thread([](std::promise<bool>& finished) {

#define TEST_TIMEOUT_FAIL_END(X)    finished.set_value(true); \
                                    }, std::ref(promisedFinished)).detach(); \
                                    EXPECT_TRUE(futureResult.wait_for(std::chrono::milliseconds(X)) != std::future_status::timeout);

#define TEST_TIMEOUT_SUCCESS_END(X) finished.set_value(true); \
                                    }, std::ref(promisedFinished)).detach(); \
                                    EXPECT_FALSE(futureResult.wait_for(std::chrono::milliseconds(X)) != std::future_status::timeout);

/**
 * This test class currently implements both the ring buffer
 * and callback approaches for memory management.
 */
class TestOSAudio : public OSAudio, public ::testing::Test {
    public:
        TestCallback *handler1;
        TestCallback *handler2;

        virtual void SetUp()
        {
            this->handler1 = new TestCallback();
            this->handler2 = new TestCallback();

            // Make sure callback list is accesible and empty
            ASSERT_EQ(this->callbackList.size(), 0);
        }

        void capture()
        {
            while (!this->deviceSwitch && this->rbs.size() > 0)
            {
                printf("Mock capturing...\n");
            }
        }

        /**
         * Mock receiving data from the OS.
         */
        void sendData()
        {
            byte *buffer = nullptr;
            for (int i = 0; i < callbackList.size(); i++)
            {
                callbackList[i]->handleData(buffer, 0);
            }
        }

        vector<Device *> getDevices(DeviceType type)
        {
            vector<Device *> devices;
            return devices;
        }

        vector<Device *> getInputDevices()
        {
            return getDevices((DeviceType)(RECORD|LOOPBACK));
        }

        vector<Device *> getOutputDevices()
        {
            return getDevices(PLAYBACK);
        }

        virtual void TearDown()
        {
            delete this->handler1;
            delete this->handler2;
        }

};

/**
 * Initialization should not create a capture thread.
 *
 * EXPECTED:
 *      Thread vectors are empty.
 */
TEST_F(TestOSAudio, init_does_not_block)
{
    EXPECT_EQ(this->inThreads.size(), 0);
    EXPECT_EQ(this->outThreads.size(), 0);
}

// Same tests for callbacks as TestController.cpp
/**
 * Add a single callback.
 *
 * EXPECTED:
 *     The callback should be present and receive data.
 */
TEST_F(TestOSAudio, singleCallback)
{
    // Add a single callback
    this->addBufferReadyCallback(this->handler1);
    ASSERT_EQ(this->callbackList.size(), 1);

    // Make sure data gets delivered
    this->sendData();
    ASSERT_TRUE(this->handler1->dataReceived);
}

/**
 * Add two callbacks.
 *
 * EXPECTED:
 *     Both callbacks should be present and receive data.
 */
TEST_F(TestOSAudio, twoCallbacks)
{
    // Add callback 1
    this->addBufferReadyCallback(this->handler1);
    ASSERT_EQ(this->callbackList.size(), 1);

    // Add callback 2
    this->addBufferReadyCallback(this->handler2);
    ASSERT_EQ(this->callbackList.size(), 2);

    // Make sure data gets delivered
    this->sendData();
    ASSERT_TRUE(this->handler1->dataReceived);
    ASSERT_TRUE(this->handler2->dataReceived);
}

/**
 * Add the same callback twice.
 *
 * EXPECTED:
 *     Callback should not be duplicated.
 */
TEST_F(TestOSAudio, duplicateCallback)
{
    this->addBufferReadyCallback(this->handler1);
    this->addBufferReadyCallback(this->handler1);
    ASSERT_EQ(this->callbackList.size(), 1);
}

/**
 * Add a callback and then remove it.
 *
 * EXPECTED:
 *     Callback should no longer be present.
 */
TEST_F(TestOSAudio, removeCallback)
{
    this->addBufferReadyCallback(this->handler1);
    this->removeBufferReadyCallback(this->handler1);

    ASSERT_EQ(this->callbackList.size(), 0);
}

/**
 * Add two callbacks then remove the first.
 *
 * EXPECTED:
 *     First callback is removed without disturbing the second.
 */
TEST_F(TestOSAudio, nonDisturbingRemoval)
{
    this->addBufferReadyCallback(this->handler1);
    this->addBufferReadyCallback(this->handler2);

    this->removeBufferReadyCallback(this->handler1);

    ASSERT_EQ(this->callbackList.size(), 1);
    ASSERT_EQ(this->callbackList[0], handler2);
}
