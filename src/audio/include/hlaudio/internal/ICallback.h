#ifndef HL_ICallback_H
#define HL_ICallback_H

namespace hula
{
    class ICallback {
        public:
            ICallback(){};
            ~ICallback(){};

            virtual void handleData(const float* data, long size) = 0;
    };
}

#endif