#include <gtest/gtest.h>
#include <hlaudio/hlaudio.h>
#include "TestCallback.h"

#define TEST_BUFFER_SIZE 0.2
#define MOCK_CAPTURE_TIME 50
#define CHECK_IF_DEAD_TIME 15

// Timeout code from: http://antonlipov.blogspot.com/2015/08/how-to-timeout-tests-in-gtest.html
// Add & to lambda capture group to allow capturing by reference
#include <future>
#define TEST_TIMEOUT_BEGIN          std::promise<bool> promisedFinished; \
                                    auto futureResult = promisedFinished.get_future(); \
                                    std::thread([&](std::promise<bool>& finished) {

#define TEST_TIMEOUT_FAIL_AFTER(X)    finished.set_value(true); \
                                    }, std::ref(promisedFinished)).detach(); \
                                    EXPECT_TRUE(futureResult.wait_for(std::chrono::milliseconds(X)) != std::future_status::timeout);

#define TEST_TIMEOUT_SUCCESS_AFTER(X) finished.set_value(true); \
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
        Device *testDevice;

        virtual void SetUp()
        {
            this->handler1 = new TestCallback();
            this->handler2 = new TestCallback();
            this->testDevice = new Device(NULL, "Device", RECORD);

            // Make sure callback list is accesible and empty
            ASSERT_EQ(this->callbackList.size(), 0);
        }

        void capture()
        {
            while (!this->deviceSwitch.load() && this->rbs.size() > 0)
            {
                printf("Mock capturing...\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));
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

        /**
         * Give everyone 4 cycles to catch up
         * This isn't guarenteed, but it has proven relatively effective
         */
        void waitForThreadDeathBeforeDestruction()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(4 * MOCK_CAPTURE_TIME));
        }

        virtual void TearDown()
        {
            delete this->handler1;
            delete this->handler2;
        }

};

/**
 * NULL device does not cause a switch.
 *
 * EXPECTED:
 *      Device does not switch.
 */
TEST_F(TestOSAudio, null_does_not_switch)
{
    setActiveInputDevice(this->testDevice);
    setActiveInputDevice(NULL);
    EXPECT_EQ(this->activeInputDevice, this->testDevice);

    waitForThreadDeathBeforeDestruction();
}

/**
 * Input device cannot be assigned an output device.
 *
 * EXPECTED:
 *      Device does not switch.
 */
TEST_F(TestOSAudio, wrong_type_does_not_switch)
{
    setActiveInputDevice(this->testDevice);

    Device *d = new Device(NULL, "Device", PLAYBACK);
    setActiveInputDevice(d);

    EXPECT_EQ(this->activeInputDevice, this->testDevice);

    waitForThreadDeathBeforeDestruction();
}

/**
 * Initialization should not create a capture thread.
 *
 * EXPECTED:
 *      Thread vectors are empty.
 *      Trying to start capture returns immediately.
 */
TEST_F(TestOSAudio, init_does_not_block)
{
    EXPECT_EQ(this->inThreads.size(), 0);
    EXPECT_EQ(this->outThreads.size(), 0);

    // This should come back immediately
    TEST_TIMEOUT_BEGIN(backgroundCapture(this));
    TEST_TIMEOUT_FAIL_AFTER(CHECK_IF_DEAD_TIME);
}

/**
 * Adding a buffer should start a capture thread.
 *
 * EXPECTED:
 *      Capture thread runs for at least a loop.
 */
TEST_F(TestOSAudio, add_starts_thread)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);
    this->addBuffer(rb);

    // Start the capture in a thread
    std::thread t(&backgroundCapture, this);

    // Join the thread
    // This should be running infinitely
    // Succeed after ~two cycles
    TEST_TIMEOUT_BEGIN(t.join());
    TEST_TIMEOUT_SUCCESS_AFTER(2 * MOCK_CAPTURE_TIME);

    this->removeBuffer(rb);
    delete rb;

    waitForThreadDeathBeforeDestruction();
}

/**
 * Removing a buffer should stop the capture thread.
 *
 * EXPECTED:
 *      Capture thread stops after buffer is removed.
 */
TEST_F(TestOSAudio, remove_kills_thread)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);
    this->addBuffer(rb);

    // Start the capture in a thread
    std::thread t(&backgroundCapture, this);

    // Wait for a cycle, then remove the buffer
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));
    this->removeBuffer(rb);

    // Join the thread
    // If it takes too long, we might still be running, so fail
    TEST_TIMEOUT_BEGIN(t.join());
    TEST_TIMEOUT_FAIL_AFTER(CHECK_IF_DEAD_TIME);

    delete rb;
}

/**
 * Triggering a device switch should stop the capture thread.
 *
 * EXPECTED:
 *      Capture thread stops after device switch is signaled.
 */
TEST_F(TestOSAudio, switch_kills_thread)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);
    this->addBuffer(rb);

    // Start the capture in a thread
    std::thread t(&backgroundCapture, this);

    // Wait for a cycle, then singal switch
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));
    setActiveInputDevice(this->testDevice);

    // Join the thread
    // If it takes too long, we might still be running, so fail
    TEST_TIMEOUT_BEGIN(t.join());
    TEST_TIMEOUT_FAIL_AFTER(CHECK_IF_DEAD_TIME);

    this->removeBuffer(rb);
    delete rb;
}

/**
 *
 * Same callback tests as Controller callbacks
 *
 */


/**
 * Add a single callback.
 *
 * EXPECTED:
 *      The callback should be present and receive data.
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
 *      Both callbacks should be present and receive data.
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
