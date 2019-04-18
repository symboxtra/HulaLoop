#ifndef HL_ICallback_H
#define HL_ICallback_H

namespace hula
{
    /**
     * Class (interface) that must be extended to create
     * and add a callback to HulaLoop.
     *
     * The class in which the callback will be implemented
     * should extend ICallback and define handleData such that
     * handleData IS the callback.
     *
     * Example class:
     * @code{.cpp}
     *
     * #include <hula/hlaudio.h>
     *
     * class Example : public ICallback {
     *      public:
     *          handleData(const SAMPLE *samples, long sampleCount)
     *          {
     *              printf("Received data.\n");
     *          }
     * }
     *
     * @endcode
     */
    class ICallback {
        public:
            ICallback(){};
            ~ICallback(){};

            /**
             * Must be implemented by the inheriting class.
             *
             * This is call VERY frequently and on some systems,
             * at the interrupt level. Do not do anything that can
             * block or is intensive. These activities should use
             * the buffer API via Controller::createAndAddBuffer().
             */
            virtual void handleData(const SAMPLE* samples, long sampleCount) = 0;
    };
}

#endif