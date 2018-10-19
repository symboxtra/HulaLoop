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

#include <cstdio>
#include <cstdlib>
#include <thread>

#include "pa_ringbuffer.h"
#include "pa_util.h"
#include "portaudio.h"


/**
 * Helper function for finding minimum value
 */
static ring_buffer_size_t rbs_min(ring_buffer_size_t a, ring_buffer_size_t b)
{
    return (a < b) ? a : b;
}

// #define SAMPLE_RATE  (17932) // Test failure to open with this value.
#define SAMPLE_RATE             (44100)
#define FRAMES_PER_BUFFER       (512)
#define NUM_SECONDS             (10)
#define NUM_CHANNELS            (2)
#define NUM_WRITES_PER_BUFFER   (4)
// #define DITHER_FLAG          (paDitherOff)
#define DITHER_FLAG             (0)

// Select sample format.
#if 1
    #define PA_SAMPLE_TYPE  paFloat32
    typedef float SAMPLE;
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

/**
* Function pointer that takes a void * as its only argument and returns void.
*/
typedef void (*rbFunction)(void *);

/**
 * Struct used for communication and data transfer between threads.
 */
typedef struct
{
    int                 threadSyncFlag;
    SAMPLE             *ringBufferData;
    PaUtilRingBuffer    ringBuffer;
    thread             *thread;
}
rbComm;

/**
 * Helper function for determining the next power of two.
 */
static unsigned NextPowerOf2(unsigned val)
{
    val--;
    val = (val >> 1) | val;
    val = (val >> 2) | val;
    val = (val >> 4) | val;
    val = (val >> 8) | val;
    val = (val >> 16) | val;
    return ++val;
}

/**
 * This routine will be called by the PortAudio engine when audio is needed.
 * It may be called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
*/
static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    rbComm *data = (rbComm *)userData;
    ring_buffer_size_t elementsWriteable = PaUtil_GetRingBufferWriteAvailable(&data->ringBuffer);
    ring_buffer_size_t elementsToWrite = rbs_min(elementsWriteable, (ring_buffer_size_t)(framesPerBuffer * NUM_CHANNELS));
    const SAMPLE *rptr = (const SAMPLE *)inputBuffer;

    // Prevent unused variable warnings.
    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    PaUtil_WriteRingBuffer(&data->ringBuffer, rptr, elementsToWrite);

    return paContinue;
}

/**
 * This routine will be called by the PortAudio engine when audio is needed.
 * It may be called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
*/
static int playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    rbComm *data = (rbComm *)userData;
    ring_buffer_size_t elementsToPlay = PaUtil_GetRingBufferReadAvailable(&data->ringBuffer);
    ring_buffer_size_t elementsToRead = rbs_min(elementsToPlay, (ring_buffer_size_t)(framesPerBuffer * NUM_CHANNELS));
    SAMPLE *wptr = (SAMPLE *)outputBuffer;

    // Prevent unused variable warnings.
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    PaUtil_ReadRingBuffer(&data->ringBuffer, wptr, elementsToRead);

    return data->threadSyncFlag ? paComplete : paContinue;
}

/**
 * Start and synchronize a thread for handling ring buffer data.
 *
 * @param pData Struct intended for communication and data transfer with thread.
 * @param fn Function that will handle ring buffer data.
 */
static PaError startThread(rbComm *pData, rbFunction fn)
{
    std::thread *t = new std::thread(fn, pData);

    // Store the thread and initialize the startup flag
    pData->thread = t;
    pData->threadSyncFlag = 1;

    // Wait for thread to startup
    while (pData->threadSyncFlag)
    {
        printf("Waiting for thread to start...\n");
        Pa_Sleep(10);
    }

    return paNoError;
}

static int stopThread(rbComm *pData)
{
    pData->threadSyncFlag = 1;

    // Wait for thread to stop
    while (pData->threadSyncFlag)
    {
        printf("Waiting for thread to stop...\n");
        Pa_Sleep(10);
    }

    // Join and destroy the thread
    pData->thread->join();
    delete pData->thread;

    return paNoError;
}

static void testWrite(void *p)
{
    rbComm *pData = (rbComm *)p;

    // Mark thread started
    pData->threadSyncFlag = 0;

    while (1)
    {
        ring_buffer_size_t elementsInBuffer = PaUtil_GetRingBufferReadAvailable(&pData->ringBuffer);
        if ((elementsInBuffer >= pData->ringBuffer.bufferSize / NUM_WRITES_PER_BUFFER) || pData->threadSyncFlag)
        {
            void *ptr[2] = {0};
            ring_buffer_size_t sizes[2] = {0};

            // By using PaUtil_GetRingBufferReadRegions, we can read directly from the ring buffer
            ring_buffer_size_t elementsRead = PaUtil_GetRingBufferReadRegions(&pData->ringBuffer, elementsInBuffer, ptr + 0, sizes + 0, ptr + 1, sizes + 1);
            if (elementsRead > 0)
            {
                printf("Read %d elements.\n", elementsRead);

                int i;
                for (i = 0; i < 2 && ptr[i] != NULL; ++i)
                {
                    // Write to standard output
                    //fwrite(ptr[i], pData->ringBuffer.elementSizeBytes, sizes[i], stdout);
                }
                PaUtil_AdvanceRingBufferReadIndex(&pData->ringBuffer, elementsRead);
            }

            if (pData->threadSyncFlag)
            {
                break;
            }
        }

        // Sleep a little while
        Pa_Sleep(20);
    }

    pData->threadSyncFlag = 0;
}