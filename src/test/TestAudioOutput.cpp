#include <gtest/gtest.h>
#include <Controller.h>
#include <stdio.h>
#include <stdlib.h>

#if _WIN32
    #include <windows.h>
#endif

#include <iostream>
using namespace std;

class TestAudioOutput : public ::testing::Test {
    public:
        vector<byte *> combinedData;
        Controller *controller = nullptr;

        virtual void SetUp()
        {
            controller = new Controller();
        }

        virtual void TearDown()
        {
            //delete controller; //TODO: Fix garbage collection
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
    ASSERT_FALSE(controller->getDevices(DeviceType::PLAYBACK).empty());

    ASSERT_FALSE(controller->getDevices((DeviceType)(DeviceType::LOOPBACK | DeviceType::RECORD)).empty());
}
