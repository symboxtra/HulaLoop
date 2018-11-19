#include <gtest/gtest.h>
#include <hlaudio/hlaudio.h>

using namespace hula;

class TestController : public Controller, public ::testing::Test {
    public:

        TestController() : Controller()
        {}

        virtual void SetUp()
        {
        }

        virtual void TearDown()
        {
        }
};

/**
 * setActiveInputDevice with a loopback device
 *
 * EXPECTED:
 *      returns true
 */
TEST_F(TestController, set_input_works_with_loopback)
{
    std::vector<Device *> devices = this->getDevices(DeviceType::LOOPBACK);

    if (devices.size() > 0)
    {
        bool success = this->setActiveInputDevice(devices[0]);
        EXPECT_TRUE(success);
    }

    Device::deleteDevices(devices);
}

/*
Disabled until LinuxAudio works

/**
 * setActiveInputDevice with a record device
 *
 * EXPECTED:
 *      returns true

TEST_F(TestController, set_input_works_with_record)
{
    std::vector<Device *> devices = this->getDevices(DeviceType::RECORD);

    if (devices.size() > 0)
    {
        bool success = this->setActiveInputDevice(devices[0]);
        EXPECT_TRUE(success);
    }

    Device::deleteDevices(devices);
}

/**
 * setActiveOutputDevice with an output device
 *
 * EXPECTED:
 *      returns true

TEST_F(TestController, set_output_works)
{
    std::vector<Device *> devices = this->getDevices(DeviceType::PLAYBACK);

    if (devices.size() > 0)
    {
        bool success = this->setActiveOutputDevice(devices[0]);
        EXPECT_TRUE(success);
    }

    Device::deleteDevices(devices);
}
*/

/**
 * setActiveInputDevice with an output device
 *
 * EXPECTED:
 *      returns false
 */
TEST_F(TestController, set_input_fails_when_given_output)
{
    std::vector<Device *> devices = this->getDevices(DeviceType::PLAYBACK);

    if (devices.size() > 0)
    {
        bool success = this->setActiveInputDevice(devices[0]);
        EXPECT_FALSE(success);
    }

    Device::deleteDevices(devices);
}

/**
 * setActiveOutputDevice with an input device
 *
 * EXPECTED:
 *      returns false
 */
TEST_F(TestController, set_output_fails_when_given_input)
{
    std::vector<Device *> devices = this->getDevices(DeviceType::RECORD);

    if (devices.size() > 0)
    {
        bool success = this->setActiveOutputDevice(devices[0]);
        EXPECT_FALSE(success);
    }

    Device::deleteDevices(devices);
}

/**
 * setActiveInputDevice with nullptr
 *
 * EXPECTED:
 *      returns false
 */
TEST_F(TestController, set_input_fails_when_given_null)
{
    bool success = this->setActiveInputDevice(nullptr);
    EXPECT_FALSE(success);
}

/**
 * setActiveOutputDevice with nullptr
 *
 * EXPECTED:
 *      returns false
 */
TEST_F(TestController, set_output_fails_when_given_null)
{
    bool success = this->setActiveOutputDevice(nullptr);
    EXPECT_FALSE(success);
}