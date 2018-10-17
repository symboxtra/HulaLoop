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

#include "hlaudio/internal/HulaRingBuffer.h"
#include "HulaAudioError.h"

/*
 * Create a new ring buffer.
 * The ring buffer's size is determined using maxDuration * sampleRate * channelCount * sizeof(SAMPLE).
 *
 * @param maxDuration The maximum length in seconds that the ring buffer should be capable of holding.
 */
HulaRingBuffer::HulaRingBuffer(float maxDuration)
{
    // Set the ring buffer size to the desired duration
    int numSamples = nextPowerOf2((uint32_t)(SAMPLE_RATE * maxDuration * NUM_CHANNELS));
    this->rbMemory = new SAMPLE[numSamples];

    // Make sure ring buffer was allocated
    if (this->rbMemory == NULL)
    {
        fprintf(stderr, "%sCould not allocate ring buffer of size %zu.\n", HL_ERROR_PREFIX, numSamples * sizeof(SAMPLE));
        exit(1);
        // TODO: Handle error
    }

    if (PaUtil_InitializeRingBuffer(&this->rb, sizeof(SAMPLE), numSamples, this->rbMemory) < 0)
    {
        fprintf(stderr, "%sFailed to initialize ring buffer. Perhaps size is not power of 2?\nSize: %d\n", HL_ERROR_PREFIX, numSamples);
        exit(1);
        // TODO: Handle error
    }
}

/*
 * Get the identifier for this ring buffer.
 *
 * @return Ring buffer's identifier.
 */
uint32_t HulaRingBuffer::getId()
{
    return 0;
}

/*
 * Read up to maxSamples from the ring buffer into the memory pointed to by data.
 *
 * @param data Pointer to allocated memory of at least maxSamples size.
 * @param maxSamples Desired number of samples.
 * @return Number of samples read.
 */
uint32_t HulaRingBuffer::read(SAMPLE * data, uint32_t maxSamples)
{
    return 0;
}

/*
 * Fetch direct pointers to memory within the ring buffer. This can be used to avoid allocating a secondary container.
 * The second pointer/size pair is for when the ring buffer has split data between its tail and head.
 * If the requested maxBytes are continuous in the underlying memory, only the first pointer/size pair is used.
 *
 * @param maxSamples Desired number of samples.
 * @param dataPtr1 The address where the first pointer should be stored.
 * @param size1 Size of the memory available from dataPtr1. THIS IS IN BYTES
 * @param dataPtr2 The address where the second pointer (if required) will be stored. NULL if not used.
 * @param size2 Size of the memory available from dataPtr2. THIS IS IN BYTES.
 * @return Number of samples read.
 */
uint32_t HulaRingBuffer::directRead(uint32_t maxSamples, void **dataPtr1, uint32_t *size1, void **dataPtr2, uint32_t *size2)
{
    return 0;
}

/*
 * Destructor for the ring buffer.
 */
HulaRingBuffer::~HulaRingBuffer()
{
    if (this->rbMemory != NULL)
    {
        PaUtil_FlushRingBuffer(&this->rb);
        delete this->rbMemory;
    }
}