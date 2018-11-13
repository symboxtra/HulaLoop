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

#include <algorithm>

#include "hlaudio/internal/HulaAudioError.h"
#include "hlaudio/internal/HulaRingBuffer.h"

/**
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

/**
 * Read up to maxSamples from the ring buffer into the memory pointed to by data.
 *
 * @param data Pointer to allocated memory of at least maxSamples size.
 * @param maxSamples Desired number of samples.
 * @return Number of samples read.
 */
ring_buffer_size_t HulaRingBuffer::read(SAMPLE *data, ring_buffer_size_t maxSamples)
{
    ring_buffer_size_t samplesRead = PaUtil_ReadRingBuffer(&this->rb, (void *)data, maxSamples);

    return samplesRead;
}

/**
 * Fetch direct pointers to memory within the ring buffer. This can be used to avoid allocating a secondary container.
 * The second pointer/size pair is for when the ring buffer has split data between its tail and head.
 * If the requested maxBytes are continuous in the underlying memory, only the first pointer/size pair is used.
 *
 * @param maxSamples Desired number of samples.
 * @param dataPtr1 The address where the first pointer should be stored.
 * @param size1 Number of elements available from dataPtr1.
 * @param dataPtr2 The address where the second pointer (if required) will be stored. NULL if not used.
 * @param size2 Number of elements available from dataPtr2.
 * @return Number of samples read.
 */
ring_buffer_size_t HulaRingBuffer::directRead(ring_buffer_size_t maxSamples, void **dataPtr1, ring_buffer_size_t *size1, void **dataPtr2, ring_buffer_size_t *size2)
{
    // Initialize
    *dataPtr1 = NULL;
    *size1 = 0;
    *dataPtr2 = NULL;
    *size2 = 0;

/*
    ring_buffer_size_t samplesInBuffer = PaUtil_GetRingBufferReadAvailable(&this->rb);
    ring_buffer_size_t samplesToRead = std::min(samplesInBuffer, maxSamples);

    // By using PaUtil_GetRingBufferReadRegions, we can read directly from the ring buffer
    ring_buffer_size_t samplesRead = PaUtil_GetRingBufferReadRegions(&this->rb, samplesToRead, dataPtr1, size1, dataPtr2, size2);
    if (samplesRead > 0)
    {
        // printf("%sDirect read of %d elements.\n", HL_PRINT_PREFIX, samplesRead);

        // Advance the index after successful read
        PaUtil_AdvanceRingBufferReadIndex(&this->rb, samplesRead);
    }
*/
     ring_buffer_size_t elementsInBuffer = PaUtil_GetRingBufferReadAvailable(&this->rb);
        void* ptr[2] = {0};
        ring_buffer_size_t sizes[2] = {0};

        /* By using PaUtil_GetRingBufferReadRegions, we can read directly from the ring buffer */
        ring_buffer_size_t elementsRead = PaUtil_GetRingBufferReadRegions(&this->rb, elementsInBuffer, dataPtr1, size1, dataPtr2, size2);
        //ring_buffer_size_t elementsRead = PaUtil_ReadRingBuffer(&pData->ringBuffer, &buffer, 512);
        if (elementsRead > 0)
        {
            //fwrite(buffer, pData->ringBuffer.elementSizeBytes, elementsRead, pData->file);
            PaUtil_AdvanceRingBufferReadIndex(&this->rb, elementsRead);
        }
    return elementsRead;
}

/**
 * Add data to the ring buffer.
 *
 * @param data Array of samples to write to the ring buffer.
 * @param maxSamples Number of samples contained in the array.
 * @return Number of samples written.
 */
ring_buffer_size_t HulaRingBuffer::write(const SAMPLE *data, ring_buffer_size_t maxSamples)
{
    /*ring_buffer_size_t elementsWriteable = PaUtil_GetRingBufferWriteAvailable(&this->rb);
    ring_buffer_size_t elementsToWrite = std::min(elementsWriteable, (ring_buffer_size_t)(maxSamples));

    ring_buffer_size_t elementsWritten = PaUtil_WriteRingBuffer(&this->rb, data, elementsToWrite);

    printf("Samples wrote: %d\n", elementsWritten);

    if (elementsWritten < maxSamples)
    {
        printf("%sOverrun: %d of %d samples written\n", HL_PRINT_PREFIX, elementsWritten, maxSamples);
    }*/
    ring_buffer_size_t elementsInBuffer = PaUtil_GetRingBufferWriteAvailable(&this->rb);

    void* ptr[2] = {0};
    ring_buffer_size_t sizes[2] = {0};

    /* By using PaUtil_GetRingBufferWriteRegions, we can write directly into the ring buffer */
    PaUtil_GetRingBufferWriteRegions(&this->rb, elementsInBuffer, ptr + 0, sizes + 0, ptr + 1, sizes + 1);

    ring_buffer_size_t elementsWritten = 0;

        ring_buffer_size_t itemsReadFromFile = 0;
        int i;
        // for (i = 0; i < 2 && ptr[i] != NULL; ++i)
        // {
        //     itemsReadFromFile += (ring_buffer_size_t)fread(ptr[i], pData->ringBuffer.elementSizeBytes, sizes[i], pData->file);
        // }
        //PaUtil_AdvanceRingBufferWriteIndex(&pData->ringBuffer, itemsReadFromFile);

    //elementsWritten = PaUtil_WriteRingBuffer(&this->rb, (void*)data, maxSamples);


    return elementsWritten;
}

/**
 * Destructor for the ring buffer.
 */
HulaRingBuffer::~HulaRingBuffer()
{
    if (this->rbMemory != NULL)
    {
        PaUtil_FlushRingBuffer(&this->rb);
        delete [] this->rbMemory;
    }
}
