#include <gtest/gtest.h>
#include <hlaudio/hlaudio.h>

#include "TestCallback.h"

using namespace hula;

#define TEST_BUFFER_SIZE 0.2f
#define MOCK_CAPTURE_TIME 25
#define CHECK_IF_DEAD_TIME 10

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

class TestOSAudio : public OSAudio, public ::testing::Test {
    public:
        Device *testDevice;

        virtual void SetUp()
        {
            this->testDevice = new Device(DeviceID(), "Device", RECORD);
            this->setActiveInputDevice(testDevice);

            // Make sure buffer and callback list is accesible and empty
            ASSERT_EQ(this->rbs.size(), 0);
            ASSERT_EQ(this->cbs.size(), 0);
        }

        void capture()
        {
            while (!this->endCapture.load())
            {
                printf("Mock capturing...\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));
            }
        }

        /**
         * Mock receiving data.
         */
        void sendData()
        {
            const SAMPLE *buffer = nullptr;
            this->doCallbacks(buffer, 0);
        }

        static bool checkThreadCount(TestOSAudio *_this)
        {
            while (_this->inThreads.size() > 0)
            { }
            return true;
        }

        std::vector<Device *> getDevices(DeviceType type)
        {
            std::vector<Device *> devices;
            return devices;
        }

        bool checkDeviceParams(Device *device)
        {
            return true;
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
            delete this->testDevice;
        }
};

/**
 * nullptr device does not cause a switch.
 *
 * EXPECTED:
 *      Device does not switch.
 */
TEST_F(TestOSAudio, null_does_not_switch)
{
    setActiveInputDevice(nullptr);
    EXPECT_EQ(this->activeInputDevice->getName(), this->testDevice->getName());

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
    Device d(DeviceID(), "Device", PLAYBACK);
    EXPECT_THROW({
        bool success = setActiveInputDevice(&d);
    }, AudioException);

    EXPECT_EQ(this->activeInputDevice->getName(), this->testDevice->getName());

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
    TEST_TIMEOUT_BEGIN(backgroundCapture());
    TEST_TIMEOUT_FAIL_AFTER(2 * MOCK_CAPTURE_TIME);
}



/*********************************************
 *                 Buffers                   *
 *********************************************/

/**
 * Add a nullptr buffer.
 *
 * EXPECTED:
 *     Buffer should not be added.
 */
TEST_F(TestOSAudio, add_nullptr_buffer)
{
    this->addBuffer(nullptr);
    ASSERT_EQ(this->rbs.size(), 0);
}

/**
 * Adding a buffer should start a capture thread.
 *
 * EXPECTED:
 *      Capture thread runs for at least a loop.
 */
TEST_F(TestOSAudio, add_buffer_starts_thread)
{
    HulaRingBuffer rb(TEST_BUFFER_SIZE);

    // Wait for a cycle, then add buffer
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));

    this->addBuffer(&rb);

    // This should be running infinitely
    // Succeed after ~two cycles
    TEST_TIMEOUT_BEGIN(backgroundCapture());
    TEST_TIMEOUT_SUCCESS_AFTER(2 * MOCK_CAPTURE_TIME);

    // OSAudio should have created a thread
    EXPECT_EQ(this->inThreads.size(), 1);

    this->removeBuffer(&rb);

    waitForThreadDeathBeforeDestruction();
}

/**
 * Removing a buffer should stop the capture thread.
 *
 * EXPECTED:
 *      Capture thread stops after buffer is removed.
 */
TEST_F(TestOSAudio, remove_buffer_kills_thread)
{
    HulaRingBuffer rb(TEST_BUFFER_SIZE);

    this->addBuffer(&rb);

    // Wait for a cycle, then remove buffer
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));

    // OSAudio should have created a thread
    EXPECT_EQ(this->inThreads.size(), 1);

    this->removeBuffer(&rb);

    waitForThreadDeathBeforeDestruction();

    // Vector isn't cleared until all threads are joined
    // Since we can't see the thread (unique ownership), use
    // this assumption to check that the thread was joined
    EXPECT_EQ(this->inThreads.size(), 0);
}



/*********************************************
 *                Callbacks                  *
 *********************************************/

/**
 * Add a single callback.
 *
 * EXPECTED:
 *     The callback should be present and receive data.
 */
TEST_F(TestOSAudio, add_single_callback)
{
    TestCallback obj1;

    // Add a single callback
    this->addCallback(&obj1);
    ASSERT_EQ(this->cbs.size(), 1);

    // Make sure data gets delivered
    this->sendData();
    ASSERT_TRUE(obj1.dataReceived);

    waitForThreadDeathBeforeDestruction();
}

/**
 * Add two callbacks.
 *
 * EXPECTED:
 *     Both callbacks should be present and receive data.
 */
TEST_F(TestOSAudio, add_two_callbacks)
{
    TestCallback obj1;
    TestCallback obj2;

    // Add callback 1
    this->addCallback(&obj1);
    ASSERT_EQ(this->cbs.size(), 1);

    // Add callback 2
    this->addCallback(&obj2);
    ASSERT_EQ(this->cbs.size(), 2);

    // Make sure data gets delivered
    this->sendData();
    ASSERT_TRUE(obj1.dataReceived);
    ASSERT_TRUE(obj2.dataReceived);

    waitForThreadDeathBeforeDestruction();
}

/**
 * Add the same callback twice.
 *
 * EXPECTED:
 *     Callback should not be duplicated.
 */
TEST_F(TestOSAudio, add_duplicate_callback)
{
    TestCallback obj1;

    this->addCallback(&obj1);
    this->addCallback(&obj1);
    ASSERT_EQ(this->cbs.size(), 1);

    waitForThreadDeathBeforeDestruction();
}

/**
 * Add the nullptr callback.
 *
 * EXPECTED:
 *     Callback should not be added.
 */
TEST_F(TestOSAudio, add_nullptr_callback)
{
    this->addCallback(nullptr);
    ASSERT_EQ(this->cbs.size(), 0);
}

/**
 * Try to remove a nonexistent callback.
 *
 * EXPECTED:
 *     no crash and other callbacks remain
 */
TEST_F(TestOSAudio, remove_nonexistent_callback)
{
    TestCallback added;
    TestCallback notAdded;

    this->addCallback(&added);
    ASSERT_EQ(this->cbs.size(), 1);

    this->removeCallback(&notAdded);

    ASSERT_EQ(this->cbs.size(), 1);
    ASSERT_EQ(this->cbs[0], &added);

    waitForThreadDeathBeforeDestruction();
}

/**
 * Add two callbacks then remove the first.
 *
 * EXPECTED:
 *     First callback is removed without disturbing the second.
 */
TEST_F(TestOSAudio, callback_nondisturbing_removal)
{
    TestCallback obj1;
    TestCallback obj2;

    this->addCallback(&obj1);
    this->addCallback(&obj2);

    this->removeCallback(&obj1);

    ASSERT_EQ(this->cbs.size(), 1);
    ASSERT_EQ(this->cbs[0], &obj2);

    waitForThreadDeathBeforeDestruction();
}

/**
 * Adding a callback should start a capture thread.
 *
 * EXPECTED:
 *      Capture thread runs for at least a loop.
 */
TEST_F(TestOSAudio, add_callback_starts_thread)
{
    TestCallback obj1;

    // Wait for a cycle, then add callback
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));

    this->addCallback(&obj1);

    // This should be running infinitely
    // Succeed after ~two cycles
    TEST_TIMEOUT_BEGIN(backgroundCapture());
    TEST_TIMEOUT_SUCCESS_AFTER(2 * MOCK_CAPTURE_TIME);

    // OSAudio should have created a thread
    EXPECT_EQ(this->inThreads.size(), 1);

    this->removeCallback(&obj1);

    waitForThreadDeathBeforeDestruction();
}

/**
 * Removing a callback should stop the capture thread.
 *
 * EXPECTED:
 *      Capture thread stops after buffer is removed.
 */
TEST_F(TestOSAudio, remove_callback_kills_thread)
{
    TestCallback obj1;
    this->addCallback(&obj1);

    // Wait for a cycle, then remove buffer
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));

    // OSAudio should have created a thread
    EXPECT_EQ(this->inThreads.size(), 1);

    this->removeCallback(&obj1);

    waitForThreadDeathBeforeDestruction();

    // Vector isn't cleared until all threads are joined
    // Since we can't see the thread (unique ownership), use
    // this assumption to check that the thread was joined
    EXPECT_EQ(this->inThreads.size(), 0);
}
