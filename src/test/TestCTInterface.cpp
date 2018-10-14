#include <gtest/gtest.h>

#include "transport.h"

class TestCTInterface : public Transport, public ::testing::Test {

};


TEST_F(TestCTInterface, checkRecord)
{
    record();
    ASSERT_EQ(stateToStr(getState()), "Recording");
}

TEST_F(TestCTInterface, checkStop)
{
    stop();
    ASSERT_EQ(stateToStr(getState()), "Stopped");
}

TEST_F(TestCTInterface, checkPlay)
{
    play();
    ASSERT_EQ(stateToStr(getState()), "Playing");
}

TEST_F(TestCTInterface, checkPause)
{
    pause();
    ASSERT_EQ(stateToStr(getState()), "Paused");
}

TEST_F(TestCTInterface, unknownEnumType)
{
    ASSERT_EQ(stateToStr((TransportState) - 1), "Unknown");
}
