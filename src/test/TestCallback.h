#ifndef TEST_CALLBACK_H
#define TEST_CALLBACK_H

#include <hlaudio/hlaudio.h>

class TestCallback : public ICallback {
    public:
        bool dataReceived;

        TestCallback()
        {
            dataReceived = false;
        }

        void handleData(byte *data, uint32_t size)
        {
            dataReceived = true;
        }
};

#endif // END TEST_CALLBACK_H