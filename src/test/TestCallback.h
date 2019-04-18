#ifndef HL_TEST_CALLBACK_H
#define HL_TEST_CALLBACK_H

 #include <hlaudio/hlaudio.h>

 using namespace hula;

 class TestCallback : public ICallback {
    public:
        bool dataReceived;

        TestCallback()
        {
            dataReceived = false;
        }

        void handleData(const SAMPLE *samples, ring_buffer_size_t sampleCount)
        {
            dataReceived = true;
        }
};

 #endif // END HL_TEST_CALLBACK_H