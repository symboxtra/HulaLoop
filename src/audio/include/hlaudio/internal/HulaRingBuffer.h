/*
 * Created using code form: https://app.assembla.com/spaces/portaudio/git/source/master/examples/paex_record_file.c
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however,
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also
 * requested that these non-binding requests be included along with the
 * license above.
 */

#ifndef HULA_RING_BUFFER_H
#define HULA_RING_BUFFER_H

#include <cstdio>
#include <cstdlib>
#include <portaudio.h>
#include <pa_ringbuffer.h>
#include <pa_util.h>
#include <thread>

#define SAMPLE_RATE             (44100)
#define FRAMES_PER_BUFFER       (512)
#define NUM_SECONDS             (10)
#define NUM_CHANNELS            (2)
#define NUM_WRITES_PER_BUFFER   (4)

// Select sample format.
#if 1
    #define PA_SAMPLE_TYPE  paFloat32
    #define SAMPLE          float
    #define SAMPLE_SILENCE  (0.0f)
    #define PRINTF_S_FORMAT "%.8f"
#elif 1
    #define PA_SAMPLE_TYPE  paInt16
    typedef short SAMPLE;
    #define SAMPLE_SILENCE  (0)
    #define PRINTF_S_FORMAT "%d"
#elif 0
    #define PA_SAMPLE_TYPE  paInt8
    typedef char SAMPLE;
    #define SAMPLE_SILENCE  (0)
    #define PRINTF_S_FORMAT "%d"
#else
    #define PA_SAMPLE_TYPE  paUInt8
    typedef unsigned char SAMPLE;
    #define SAMPLE_SILENCE  (128)
    #define PRINTF_S_FORMAT "%d"
#endif

#define BYTES_TO_SAMPLES(bytes) ((bytes) / (sizeof(SAMPLE)))
#define SAMPLES_TO_BYTES(samples) ((samples) * (sizeof(SAMPLE)))

/*
 * HulaLoop wrapper class for PortAudio ring buffer.
 */
class HulaRingBuffer {

    private:
        /*
         * Underlying memory allocated for the ring buffer.
         */
        SAMPLE *rbMemory;

        /*
         * PortAudio ring buffer structure.
         */
        PaUtilRingBuffer rb;

        /*
        * Helper function for determining the next power of two.
        *
        * @return Next power of 2 from val
        */
        static uint32_t nextPowerOf2(uint32_t val) {
            val--;
            val = (val >> 1) | val;
            val = (val >> 2) | val;
            val = (val >> 4) | val;
            val = (val >> 8) | val;
            val = (val >> 16) | val;
            return ++val;
        };

    public:
        HulaRingBuffer(float maxDuration);

        int32_t read(SAMPLE * data, int32_t maxSamples);
        int32_t directRead(int32_t maxSamples, void **dataPtr1, int32_t *size1, void **dataPtr2, int32_t *size2);
        int32_t write(const SAMPLE * data, int32_t maxSamples);

        ~HulaRingBuffer();

};

#endif // END HULA_RING_BUFFER_H