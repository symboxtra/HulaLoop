#include <gtest/gtest.h>
#include <hlaudio/hlaudio.h>

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
            this->testDevice = new Device(NULL, "Device", RECORD);
        }

        void capture()
        {
            while (!this->endCapture.load())
            {
                printf("Mock capturing...\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));
            }
        }

        static bool checkThreadCount(TestOSAudio *_this)
        {
            while (_this->inThreads.size() > 0)
            { }
            return true;
        }

        vector<Device *> getDevices(DeviceType type)
        {
            vector<Device *> devices;
            return devices;
        }

        bool checkRates(Device *device)
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
    TEST_TIMEOUT_FAIL_AFTER(2 * MOCK_CAPTURE_TIME);
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

    // This should be running infinitely
    // Succeed after ~two cycles
    TEST_TIMEOUT_BEGIN(backgroundCapture(this));
    TEST_TIMEOUT_SUCCESS_AFTER(2 * MOCK_CAPTURE_TIME);

    // OSAudio should have created a thread
    EXPECT_EQ(this->inThreads.size(), 1);

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

    // Wait for a cycle, then remove buffer
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));

    // OSAudio should have created a thread
    EXPECT_EQ(this->inThreads.size(), 1);

    this->removeBuffer(rb);
    delete rb;

    waitForThreadDeathBeforeDestruction();

    // Vector isn't cleared until all threads are joined
    // Since we can't see the thread (unique ownership), use
    // this assumption to check that the thread was joined
    EXPECT_EQ(this->inThreads.size(), 0);
}

/**
 * DISABLED: Disabled since this won't pass consistently.
 * The window in which we need to check if the thread vector is
 * empty is too small.
 *
 * Triggering a device switch should stop the capture thread.
 *
 * EXPECTED:
 *      Capture thread stops after device switch is signaled.
 */
/*
TEST_F(TestOSAudio, switch_kills_thread)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);
    this->addBuffer(rb);

    // Wait for a cycle, then singal switch
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CAPTURE_TIME));

    // OSAudio should have created a thread
    EXPECT_EQ(this->inThreads.size(), 1);

    // Start checking before we go to switch
    std::thread t(&checkThreadCount, this);

    // Switch device
    setActiveInputDevice(this->testDevice);

    // Make sure the threads actually died
    TEST_TIMEOUT_BEGIN(t.join());
    TEST_TIMEOUT_FAIL_AFTER(2 * MOCK_CAPTURE_TIME);

    this->removeBuffer(rb);
    delete rb;

    waitForThreadDeathBeforeDestruction();
}
*/
