#include <gtest/gtest.h>
#include <hlaudio/hlaudio.h>

#include "../audio/OSXAudio.h"

using namespace hula;

// Don't extend OSXAudio.
// Use a shared instance of it since setup is so expensive.
// This is black-box testing of OSXAudio since we can't extend
// without creating a new instance each time.
class TestOSXAudio : public ::testing::Test {
    public:
        static OSXAudio *audio;

        /**
         * Since setup is so expensive, use a shared instance of OSXAudio.
         * This will only be called once.
         */
        static void SetUpTestCase()
        {
            audio = new OSXAudio();
        }

        /**
         * Only called once at end of all tests.
         */
        static void TearDownTestCase()
        {
            delete audio;
        }
};

OSXAudio *TestOSXAudio::audio = NULL;

/**
 * Get input (record or loopback) devices.
 *
 * EXPECTED:
 *      Device list is not empty.
 *      All returned devices are of the correct type.
 */
TEST_F(TestOSXAudio, get_input_devices)
{
    std::vector<Device *> devs = audio->getDevices((DeviceType)(RECORD | LOOPBACK));
    EXPECT_GT(devs.size(), 0);

    for (int i = 0; i < devs.size(); i++)
    {
        // Device should be record or loopback
        EXPECT_TRUE(devs[i]->getType() & RECORD || devs[i]->getType() & LOOPBACK);
    }
    Device::deleteDevices(devs);
}

/**
 * Get input (only record) devices.
 *
 * EXPECTED:
 *      Device list is not empty.
 *      All returned devices are of the correct type.
 */
TEST_F(TestOSXAudio, get_record_devices)
{
    std::vector<Device *> devs = audio->getDevices(DeviceType::RECORD);
    EXPECT_GT(devs.size(), 0);

    for (int i = 0; i < devs.size(); i++)
    {
        // Device should be record
        EXPECT_TRUE(devs[i]->getType() & RECORD);
    }
    Device::deleteDevices(devs);
}

/**
 * Get input (only loopback) devices.
 *
 * EXPECTED:
 *      Device list is not empty.
 *      All returned devices are of the correct type.
 */
TEST_F(TestOSXAudio, get_loopback_devices)
{
    std::vector<Device *> devs = audio->getDevices(DeviceType::LOOPBACK);
    EXPECT_GT(devs.size(), 0);

    for (int i = 0; i < devs.size(); i++)
    {
        // Device should be loopback
        EXPECT_TRUE(devs[i]->getType() & DeviceType::LOOPBACK);
    }
    Device::deleteDevices(devs);
}

/**
 * Get output (playback) devices.
 *
 * EXPECTED:
 *      Device list is not empty.
 *      All returned devices are of the correct type.
 */
TEST_F(TestOSXAudio, get_output_devices)
{
    std::vector<Device *> devs = audio->getDevices(DeviceType::PLAYBACK);
    EXPECT_GT(devs.size(), 0);

    for (int i = 0; i < devs.size(); i++)
    {
        // Device should be playback
        EXPECT_TRUE(devs[i]->getType() & DeviceType::PLAYBACK);
    }
    Device::deleteDevices(devs);
}

/**
 * Get devices with an invalid enum.
 *
 * EXPECTED:
 *      Device list should be empty.
 */
TEST_F(TestOSXAudio, get_invalid_type_devices)
{
    std::vector<Device *> devs = audio->getDevices((DeviceType)(0));
    EXPECT_EQ(devs.size(), 0);
    Device::deleteDevices(devs);
}

/**
 * Run a short capture test.
 *
 * EXPECTED:
 *      Capture starts.
 *      Capture writes to our ring buffer.
 *      Capture ends.
 *      Data is not deleted.
 */
TEST_F(TestOSXAudio, short_capture)
{
    int maxSamples = 25;
    SAMPLE *readData = new SAMPLE[maxSamples];

    HulaRingBuffer *rb = new HulaRingBuffer(0.5);
    audio->addBuffer(rb);

    // Read some samples
    // Succeed if we get something
    int32_t samplesRead = 0;
    for (int i = 0; i < 50; i++)
    {
        samplesRead = rb->read(readData, maxSamples);
        if (samplesRead > 0)
        {
            break;
        }

        // Sleep for a bit to let some data flow in
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    EXPECT_GT(samplesRead, 0);

    // Remove the buffer
    audio->removeBuffer(rb);
    delete rb;
    delete [] readData;
}
