#include <gtest/gtest.h>
#include <hlcontrol/hlcontrol.h>

#include <sys/stat.h>

using namespace hula;

class TestTransport : public Transport, public ::testing::Test {

    public:
        TestTransport() : Transport()
        { }

};

TEST_F(TestTransport, verifyController)
{
    ASSERT_FALSE(getController() == nullptr);
}

/**
 * Test transport control functions (happy paths)
 */

TEST_F(TestTransport, checkRecord)
{
    ASSERT_TRUE(record());
    ASSERT_EQ(stateToStr(getState()), "Recording");

    ASSERT_TRUE(stop());
    ASSERT_EQ(stateToStr(getState()), "Stopped");
}

TEST_F(TestTransport, checkPausePlay)
{
    ASSERT_TRUE(record());
    ASSERT_EQ(stateToStr(getState()), "Recording");

    ASSERT_TRUE(stop());
    ASSERT_EQ(stateToStr(getState()), "Stopped");

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

TEST_F(TestTransport, invalid_controls)
{
    ASSERT_FALSE(stop());
    ASSERT_FALSE(pause());
    ASSERT_FALSE(play());

    // Test invalid record
    ASSERT_TRUE(record());
    ASSERT_FALSE(play());
    ASSERT_TRUE(stop());
    ASSERT_FALSE(record());
}

TEST_F(TestTransport, verify_tempfile_deletion)
{
    for (int i = 0; i < 4; i++)
    {
        ASSERT_TRUE(record());

        // Sleep for a second to prevent file name clash during creation
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        ASSERT_TRUE(pause());
    }

    // Delete temp files and check if vector is empty
    std::vector<std::string> dirs = recorder->getExportPaths();
    discard();
    ASSERT_EQ(recorder->getExportPaths().size(), 0);

    struct stat buffer;
    for (int i = 0; i < dirs.size(); i++)
    {
        ASSERT_NE(stat(dirs[i].c_str(), &buffer), 0);
    }
}