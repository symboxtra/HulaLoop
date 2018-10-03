#include <gtest/gtest.h>
#include <ICallback.h>
#include <Controller.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

class TestAudioOutput : public ICallback, public ::testing::Test
{
    public:
    vector<byte *> combinedData;
    Controller * controller = nullptr;

    virtual void SetUp()
    {

        int argc = 0;
        char **argv = nullptr;
        controller = new Controller();
        controller->addBufferReadyCallback(this);
    }

    virtual void TearDown()
    {
        controller->removeBufferReadyCallback(this);
        delete controller;
    }

    void handleData(byte *data, uint32_t size)
    {
        combinedData.push_back(data);
    }
};

TEST_F(TestAudioOutput, checkAudioOutput)
{
    system("play ../src/test/test.wav");
    sleep(25);
    ASSERT_FALSE(combinedData.empty());
}