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