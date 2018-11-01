#include <gtest/gtest.h>
#include <hlcontrol/hlcontrol.h>

class TestRecord : public ::testing::Test {
    public:
        /**
         * Create the Transport in "dry run" mode.
         * No backend audio services or devices are initialized.
         */
        virtual void SetUp()
        {

        }

        virtual void TearDown()
        {

        }

};

TEST_F(TestRecord, checkIfRecording)
{
    Transport * t = new Transport();
    bool isRecording = t->record();
    bool isStateRecording = (t->stateToStr(t->getState()) == "Recording");
    ASSERT_EQ(isRecording, isStateRecording);
}

TEST_F(TestRecord, checkIfStopped)
{
    Transport * t = new Transport();
    bool isStopped = t->stop();
    bool isStateStopped = (t->stateToStr(t->getState()) == "Stopped");
    ASSERT_EQ(isStopped, isStateStopped);
}