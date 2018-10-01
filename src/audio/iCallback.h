#ifndef I_CALLBACK
#define I_CALLBACK

using byte = uint8_t;

class iCallback
{
    public:
        iCallback(){};
        ~iCallback(){};

        virtual void handleData(uint32_t size, byte* data) = 0;
};

#endif