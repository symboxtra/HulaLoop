#ifndef I_CALLBACK
#define I_CALLBACK

using byte = uint8_t;

/**
 * Callback "Interface" used to add and remove to callback list
 */
// TODO: Add a better public description
class ICallback {
    public:
        ICallback() {};
        ~ICallback() {};

        virtual void handleData(byte *data, uint32_t size) = 0;
};

#endif