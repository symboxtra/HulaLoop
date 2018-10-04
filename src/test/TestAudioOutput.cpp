#include <gtest/gtest.h>
#include <ICallback.h>
#include <Controller.h>
#include <stdio.h>
#include <stdlib.h>

#if _WIN32
    #include <windows.h>
#endif

#include <iostream>
using namespace std;

class TestAudioOutput : public ICallback, public ::testing::Test
{
    public:
    vector<byte *> combinedData;
    Controller * controller = nullptr;

    virtual void SetUp()
    {
        controller = new Controller();
        controller->addBufferReadyCallback(this);
    }

    virtual void TearDown()
    {
        controller->removeBufferReadyCallback(this);
        //delete controller; //TODO: Fix garbage collection
    }

    void handleData(byte *data, uint32_t size)
    {
        combinedData.push_back(data);
    }
};

TEST_F(TestAudioOutput, checkAudioOutput)
{
    #if _WIN32
        system("start powershell.exe -Wait  (New-Object Media.SoundPlayer (Resolve-Path ../src/test/test.wav)).PlaySync()");
        Sleep(25000);
    #elif __APPLE__

    #elif __unix__
        system("timeout 5 | play ../src/test/test.wav");
    #endif

    ASSERT_FALSE(combinedData.empty());
}

TEST_F(TestAudioOutput, checkDeviceList)
{
    ASSERT_FALSE(controller->getOutputDevices().empty());

    ASSERT_FALSE(controller->getInputDevices().empty());
}
