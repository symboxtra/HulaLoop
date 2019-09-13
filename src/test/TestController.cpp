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

/**
 * setActiveInputDevice with a record device
 *
 * EXPECTED:
 *      returns true
 */
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
 */
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

/**
 * setActiveInputDevice with an output device
 *
 * EXPECTED:
 *      throws exception
 */
TEST_F(TestController, set_input_fails_when_given_output)
{
    Device device(DeviceID(), "Device", DeviceType::PLAYBACK);

    EXPECT_THROW({
        bool success = this->setActiveInputDevice(&device);
    }, AudioException);
}

/**
 * setActiveOutputDevice with an input device
 *
 * EXPECTED:
 *      throws exception
 */
TEST_F(TestController, set_output_fails_when_given_input)
{
    Device device(DeviceID(), "Device", DeviceType::RECORD);

    EXPECT_THROW({
        bool success = this->setActiveOutputDevice(&device);
    }, AudioException);
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

/**
 * addBuffer with nullptr
 *
 * EXPECTED:
 *      no crash
 */
TEST_F(TestController, addBuffer_handles_null)
{
    this->addBuffer(nullptr);
}

/**
 * removeBuffer with nullptr
 *
 * EXPECTED:
 *      no crash
 */
TEST_F(TestController, removeBuffer_handles_null)
{
    this->removeBuffer(nullptr);
}

/**
 * addCallback with nullptr
 *
 * EXPECTED:
 *      no crash
 */
TEST_F(TestController, addCallback_handles_null)
{
    this->addCallback(nullptr);
}

/**
 * removeCallback with nullptr
 *
 * EXPECTED:
 *      no crash
 */
TEST_F(TestController, removeCallback_handles_null)
{
    this->removeCallback(nullptr);
}