#include <gtest/gtest.h>
#include <hlaudio/hlaudio.h>

class TestController : public Controller, public ::testing::Test {
    public:

        TestController() : Controller(true)
        {}

        virtual void SetUp()
        {
        }

        virtual void TearDown()
        {
        }

};