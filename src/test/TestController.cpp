#include <gtest/gtest.h>

#include <Controller.h>
#include <ICallback.h>


class TestCallback : public ICallback {
    public:
        bool dataReceived;

        TestCallback()
        {
            dataReceived = false;
        }

        void handleData(byte *data, uint32_t size)
        {
            dataReceived = true;
        }
};

class TestController : public Controller, public ::testing::Test {
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

        /**
         * Trigger a buffer dispatch from the Controller.
         */
        void sendData()
        {
            byte *buffer = nullptr;
            this->handleData(buffer, 0);
        }

        virtual void TearDown()
        {
            delete this->handler1;
            delete this->handler2;
        }

};

/**
 * Add a single callback.
 *
 * EXPECTED:
 *     The callback should be present and receive data.
 */
TEST_F(TestController, singleCallback)
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
TEST_F(TestController, twoCallbacks)
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
TEST_F(TestController, duplicateCallback)
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
TEST_F(TestController, removeCallback)
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
TEST_F(TestController, nonDisturbingRemoval)
{
    this->addBufferReadyCallback(this->handler1);
    this->addBufferReadyCallback(this->handler2);

    this->removeBufferReadyCallback(this->handler1);

    ASSERT_EQ(this->callbackList.size(), 1);
    ASSERT_EQ(this->callbackList[0], handler2);
}
