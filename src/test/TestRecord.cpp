#include <gtest/gtest.h>
#include <hlcontrol/hlcontrol.h>

using namespace hula;

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
    ASSERT_EQ(2, 2);
}

TEST_F(TestRecord, checkIfStopped)
{
    ASSERT_EQ(3, 3);
}