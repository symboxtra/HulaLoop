#include <gtest/gtest.h>
#include <hlaudio/hlaudio.h>

using namespace hula;

#define TEST_BUFFER_SIZE 0.2f
#define TEST_NUM_SAMPLES 200 // Don't pick a power of 2
SAMPLE sampleData = (SAMPLE)7.7;

/**
 * Verify that each sample in the given array is the same
 * as the sample data.
 *
 * @return True if all samples matched. False if one or more failed.
 */
bool verifySamples(SAMPLE *samples, int numSamples)
{
    for (int i = 0; i < numSamples; i++)
    {
        SAMPLE val = sampleData * i;
        if (memcmp(samples + i, &val, sizeof(SAMPLE)))
        {
            return false;
        }
    }
    return true;
}

/**
 * Create an array of samples for writing into the ring buffer.
 * Each sample contains the float 7.7 or some subsection of its
 * memory for smaller types.
 *
 * The returned array must be DELETED.
 * Ex: delete [] data;
 *
 * @return Array of TEST_NUM_SAMPLES samples.
 */
SAMPLE *createTestSamples()
{
    // This needs to change when we un-define SAMPLE
    // Assume 32-bit float is biggest sample
    // Anything smaller will fit within
    SAMPLE *samples = new SAMPLE[TEST_NUM_SAMPLES];

    for (int i = 0; i < TEST_NUM_SAMPLES; i++)
    {
        SAMPLE val = sampleData * i;
        memcpy(samples + i, &val, sizeof(SAMPLE));
    }

    if (!verifySamples(samples, TEST_NUM_SAMPLES))
    {
        ADD_FAILURE();
    }

    return samples;
}

/**
 * Create a valid ring buffer.
 * Destroy the ring buffer.
 *
 * EXPECTED:
 *      Return is non-null.
 *      Destructor does not explode.
 */
TEST(TestHulaRingBuffer, create_and_destroy_buffer)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);
    ASSERT_TRUE(rb != nullptr);

    delete rb;
}

/**
 * Write to a ring buffer.
 *
 * EXPECTED:
 *      Write does not fail.
 *      Write writes all samples.
 */
TEST(TestHulaRingBuffer, write_buffer)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);

    SAMPLE *writeData = createTestSamples();

    ring_buffer_size_t samplesWritten = rb->write(writeData, TEST_NUM_SAMPLES);
    EXPECT_EQ(samplesWritten, TEST_NUM_SAMPLES);

    delete [] writeData;
    delete rb;
}

/**
 * Write to a ring buffer.
 * Read from the buffer.
 *
 * EXPECTED:
 *      Write does not fail.
 *      Read does not fail.
 *      Write writes all samples.
 *      Read reads all samples.
 *      Data is intact.
 */
TEST(TestHulaRingBuffer, read_buffer)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);

    SAMPLE *writeData = createTestSamples();
    SAMPLE *readData = new SAMPLE[TEST_NUM_SAMPLES];

    ring_buffer_size_t samplesWritten = rb->write(writeData, TEST_NUM_SAMPLES);
    EXPECT_EQ(samplesWritten, TEST_NUM_SAMPLES);

    ring_buffer_size_t samplesRead = rb->read(readData, TEST_NUM_SAMPLES);
    EXPECT_EQ(samplesRead, TEST_NUM_SAMPLES);

    // Make sure the two are identical
    EXPECT_TRUE(verifySamples(readData, TEST_NUM_SAMPLES));

    delete [] readData;
    delete [] writeData;
    delete rb;
}

/**
 * Write to a ring buffer.
 * Read less samples than we wrote.
 *
 * EXPECTED:
 *      Write does not fail.
 *      Read does not fail.
 *      Write writes all samples.
 *      Read reads the number of samples that we requested.
 *      Data is intact.
 */
TEST(TestHulaRingBuffer, read_less_than_we_wrote)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);

    SAMPLE *writeData = createTestSamples();
    SAMPLE *readData = new SAMPLE[TEST_NUM_SAMPLES / 2];

    ring_buffer_size_t samplesWritten = rb->write(writeData, TEST_NUM_SAMPLES);
    EXPECT_EQ(samplesWritten, TEST_NUM_SAMPLES);

    ring_buffer_size_t samplesRead = rb->read(readData, TEST_NUM_SAMPLES / 2);
    EXPECT_EQ(samplesRead, TEST_NUM_SAMPLES / 2);

    // Make sure the two are identical
    EXPECT_TRUE(verifySamples(readData, TEST_NUM_SAMPLES / 2));

    delete [] readData;
    delete [] writeData;
    delete rb;
}

/**
 * Write to a ring buffer.
 * Try to read more samples than we wrote.
 *
 * EXPECTED:
 *      Write does not fail.
 *      Read does not fail.
 *      Write writes all samples.
 *      Read reads the number of samples that we wrote.
 */
TEST(TestHulaRingBuffer, read_more_than_we_wrote)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);

    SAMPLE *writeData = createTestSamples();
    SAMPLE *readData = new SAMPLE[TEST_NUM_SAMPLES];

    ring_buffer_size_t samplesWritten = rb->write(writeData, TEST_NUM_SAMPLES);
    EXPECT_EQ(samplesWritten, TEST_NUM_SAMPLES);

    ring_buffer_size_t samplesRead = rb->read(readData, TEST_NUM_SAMPLES * 2);
    EXPECT_EQ(samplesRead, TEST_NUM_SAMPLES);

    delete [] readData;
    delete [] writeData;
    delete rb;
}

/**
 * Write to a ring buffer.
 * Directly read from the ring buffer.
 *
 * The data here is small enough that we should only get one pointer back.
 *
 * EXPECTED:
 *      Write does not fail.
 *      Read returns valid pointer.
 *      Write writes all samples.
 */
TEST(TestHulaRingBuffer, direct_read_buffer)
{
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);

    SAMPLE *writeData = createTestSamples();

    ring_buffer_size_t samplesWritten = rb->write(writeData, TEST_NUM_SAMPLES);
    EXPECT_EQ(samplesWritten, TEST_NUM_SAMPLES);

    void *ptr1 = &rb;  // Random address
    ring_buffer_size_t count1 = 7;
    void *ptr2 = &rb;  // Random address
    ring_buffer_size_t count2 = 7;

    ring_buffer_size_t samplesRead = rb->directRead(TEST_NUM_SAMPLES, &ptr1, &count1, &ptr2, &count2);
    EXPECT_EQ(samplesRead, TEST_NUM_SAMPLES);

    // Should be data in the first pair
    EXPECT_TRUE(ptr1 != nullptr);
    EXPECT_EQ(count1, TEST_NUM_SAMPLES);

    // Should have cleared the second pair
    EXPECT_TRUE(ptr2 == nullptr);
    EXPECT_EQ(count2, 0);

    // Make sure the two are identical
    EXPECT_TRUE(verifySamples((SAMPLE *)ptr1, TEST_NUM_SAMPLES));

    delete [] writeData;
    delete rb;
}

/**
 * Write lots of data to the buffer.
 * Make sure the pointer wraps as expected.
 *
 * EXPECTED:
 *      The second pointer to directRead becomes non-null.
 *      Data is intact accross wrap.
 */
TEST(TestHulaRingBuffer, direct_read_wrap_buffer)
{
    // Small buffer
    HulaRingBuffer *rb = new HulaRingBuffer(TEST_BUFFER_SIZE);

    SAMPLE *writeData = createTestSamples();

    void *ptr1 = nullptr;  // Random address
    ring_buffer_size_t count1 = 0;
    void *ptr2 = nullptr;  // Random address
    ring_buffer_size_t count2 = 0;

    // Read/write until we reach the wrap around
    while (count2 == 0)
    {
        ring_buffer_size_t samplesWritten = rb->write(writeData, TEST_NUM_SAMPLES);
        EXPECT_EQ(samplesWritten, TEST_NUM_SAMPLES);

        ring_buffer_size_t samplesRead = rb->directRead(TEST_NUM_SAMPLES, &ptr1, &count1, &ptr2, &count2);
        EXPECT_EQ(samplesRead, TEST_NUM_SAMPLES);
    }

    // Should be data in the first pair
    EXPECT_TRUE(ptr1 != nullptr);
    EXPECT_GT(count1, 0);

    // Should be data in the second pair
    EXPECT_TRUE(ptr2 != nullptr);
    EXPECT_GT(count2, 0);

    // Sizes should sum
    EXPECT_EQ(count1 + count2, TEST_NUM_SAMPLES);

    // Dump both to continuous memory
    SAMPLE *readData = new SAMPLE[TEST_NUM_SAMPLES];
    memcpy(readData, ptr1, SAMPLES_TO_BYTES(count1));
    memcpy(readData + count1, ptr2, SAMPLES_TO_BYTES(count2));

    // Make sure the two are identical
    EXPECT_TRUE(verifySamples(readData, TEST_NUM_SAMPLES));

    delete [] readData;
    delete [] writeData;
    delete rb;
}