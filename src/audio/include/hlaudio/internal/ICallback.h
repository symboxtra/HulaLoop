#ifndef HL_ICallback_H
#define HL_ICallback_H

namespace hula
{
    class ICallback {
        public:
            ICallback();
            ~ICallback();

            virtual void handleData(uint8_t* data, uint32_t size) = 0;
    };
}

#endif