#include <gtest/gtest.h>
#include <hlcontrol/hlcontrol.h>

#include <sys/stat.h>

using namespace hula;

#define MOCK_CONTROL_TIME 200

class TestCTInterface : public Transport, public ::testing::Test {

    public:
        /**
         * Create the Transport in "dry run" mode.
         * No backend audio services or devices are initialized.
         */
        TestCTInterface() : Transport()
        { }

};

TEST_F(TestCTInterface, verifyController)
{
    ASSERT_FALSE(getController() == NULL);
}

/**
 * Test transport control functions (happy paths)
 *
 * There is a race condition when controls are switched too quickly
 * MOCK_CONTROL_TIME ms is used after a record call to pause the thread to simulate a human performing these actions
 */

TEST_F(TestCTInterface, checkRecord)
{
    ASSERT_TRUE(record());
    ASSERT_EQ(stateToStr(getState()), "Recording");
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CONTROL_TIME));

    ASSERT_TRUE(stop());
    ASSERT_EQ(stateToStr(getState()), "Stopped");
}

TEST_F(TestCTInterface, checkPausePlay)
{
    ASSERT_TRUE(record());
    ASSERT_EQ(stateToStr(getState()), "Recording");
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CONTROL_TIME));

    ASSERT_TRUE(pause());
    ASSERT_EQ(stateToStr(getState()), "Paused");

    ASSERT_TRUE(play());
    ASSERT_EQ(stateToStr(getState()), "Playing");

    ASSERT_TRUE(pause());
    ASSERT_EQ(stateToStr(getState()), "Paused");

    ASSERT_TRUE(stop());
    ASSERT_EQ(stateToStr(getState()), "Stopped");
}

/**
 * Test invalid transport control calls and make sure that
 */

TEST_F(TestCTInterface, invalid_controls)
{
    ASSERT_FALSE(stop());
    ASSERT_FALSE(pause());
    ASSERT_FALSE(play());

    // Test invalid record
    ASSERT_TRUE(record());
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CONTROL_TIME));
    ASSERT_TRUE(stop());
    ASSERT_FALSE(record());

}

TEST_F(TestCTInterface, verify_tempfile_deletion)
{
    for(int i = 0;i < 4;i++)
    {
        ASSERT_TRUE(record());

        // Sleep for a second to prevent file name clash during creation
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        ASSERT_TRUE(pause());
    }

    // Delete temp files and check if vector is empty
    std::vector<std::string> dirs = recorder->getExportPaths();
    deleteTempFiles();
    ASSERT_EQ(recorder->getExportPaths().size(), 0);

    struct stat buffer;
    for(int i = 0;i < dirs.size();i++)
    {
        ASSERT_NE(stat(dirs[i].c_str(), &buffer), 0);
    }
}