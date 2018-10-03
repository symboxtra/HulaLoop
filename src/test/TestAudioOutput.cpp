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
    pthread_mutex_t mutex;

    virtual void SetUp()
    {
        pthread_mutex_init(&mutex, NULL);
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
        pthread_mutex_lock(&mutex);
        combinedData.push_back(data);
        pthread_mutex_unlock(&mutex);
    }
};

TEST_F(TestAudioOutput, checkAudioOutput)
{
    system("play ../src/test/test.wav");
    ASSERT_FALSE(combinedData.empty());
}
