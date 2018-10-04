#include <gtest/gtest.h>

#include "transport.h"

class TestCTInterface : public Transport, public ::testing::Test
{
        
};


TEST_F(TestCTInterface, checkRecord)
{
    // Transport t;
    record();
    ASSERT_TRUE(stateToStr(getState()) == "Recording");
}

TEST_F(TestCTInterface, checkStop)
{
    // Transport t;
    stop();
    ASSERT_TRUE(stateToStr(getState()) == "Stopped");
}

TEST_F(TestCTInterface, checkPlay)
{
    // Transport t;
    play();
    ASSERT_TRUE(stateToStr(getState()) == "Playing");
}

TEST_F(TestCTInterface, checkPause)
{
    // Transport t;
    pause();
    ASSERT_TRUE(stateToStr(getState()) == "Paused");
}
