#include <gtest/gtest.h>
#include <hlaudio/hlaudio.h>

#include "../audio/WindowsAudio.h"

using namespace hula;

// Don't extend WindowsAudio.
// Use a shared instance of it since setup is so expensive.
// This is black-box testing of WindowsAudio since we can't extend
// without creating a new instance each time.
class TestWindowsAudio : public ::testing::Test {
    public:
        static WindowsAudio *audio;

        /**
         * Since setup is so expensive, use a shared instance of WindowsAudio.
         * This will only be called once.
         */
        static void SetUpTestCase()
        {
            audio = new WindowsAudio();
        }

        /**
         * Only called once at end of all tests.
         */
        static void TearDownTestCase()
        {
            delete audio;
        }
};

WindowsAudio *TestWindowsAudio::audio = NULL;
std::string path = "";

/**
 * Get input (record or loopback) devices.
 *
 * EXPECTED:
 *      Device list is not empty.
 *      All returned devices are of the correct type.
 */
TEST_F(TestWindowsAudio, get_input_devices)
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
TEST_F(TestWindowsAudio, get_record_devices)
{
    std::vector<Device *> devs = audio->getDevices(DeviceType::RECORD);
    EXPECT_GE(devs.size(), 0);

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
TEST_F(TestWindowsAudio, get_loopback_devices)
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
TEST_F(TestWindowsAudio, get_output_devices)
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
TEST_F(TestWindowsAudio, get_invalid_type_devices)
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
 *      Audio writes to our ring buffer.
 *      Capture ends.
 *      Data is not deleted.
 */
TEST_F(TestWindowsAudio, short_capture)
{
    int maxSamples = 100;
    SAMPLE *readData = new SAMPLE[maxSamples];

    HulaRingBuffer *rb = new HulaRingBuffer(5);
    audio->addBuffer(rb);

    // Sleep for a few seconds to allow thread to start
    // and data to flow in
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::string command = "PowerShell -Command \"(New-Object Media.SoundPlayer (Resolve-Path \'" + path + "test.wav\')).PlaySync()\"";
    system(command.c_str());

    // Read some samples
    ring_buffer_size_t samplesRead = rb->read(readData, maxSamples);
    EXPECT_EQ(samplesRead, maxSamples);

    // Remove the buffer
    audio->removeBuffer(rb);
    delete rb;
    delete [] readData;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // Parse the path to the test executable
    // Powershell's Resolve-Path will expand relative paths later
    path = std::string(argv[0]);
    size_t lastBSlash = path.find_last_of("\\");
    size_t lastFSlash = path.find_last_of("/");

    // Strip the executable name
    if (lastBSlash != std::string::npos)
        path = path.substr(0, lastBSlash + 1);
    else if (lastFSlash != std::string::npos)
        path = path.substr(0, lastFSlash + 1);

    return RUN_ALL_TESTS();
}
