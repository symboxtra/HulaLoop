#include <portaudio.h>

#include <cstdint>
#include <iostream>
#include <thread>

#include "OSXAudio.h"
#include "OSXDaemon/OSXDaemon.h"
#include "OSXRingBuffer.h"
#include "HulaAudioError.h"

/**
 * Constructs an instance of OSXAudio class.
 *
 * This will initialize an OSXDaemon which will handle the virtual loopback.
 * OSXDaemon will join or start a JACK server to get audio.
 */
OSXAudio::OSXAudio()
{
    // Create the loopback daemon
    OSXDaemon *osxDaemon;
    osxDaemon = new OSXDaemon("HulaLoop #1", 0); // TODO: Make sure this can handle multiple instances of HulaLoop running
    osxDaemon->activate();
    osxDaemon->monitor();

    printf("\n\n");

    // Initialize PortAudio
    int ret = Pa_Initialize();
    if (ret != paNoError)
    {
        fprintf(stderr, "%sPortAudio failed to initialize.\nError code: 0x%x\n", HL_ERROR_PREFIX, ret);
        exit(1); // TODO: Handle error
    }

    printf("\n\n");

    // Start a test capture
    std::thread(&test_capture, this).detach();
}

/**
 * Execution loop for loopback capture
 *
 * Based on code from https://app.assembla.com/spaces/portaudio/git/source/master/examples/paex_record_file.c
 */
void OSXAudio::capture()
{
    PaStreamParameters  inputParameters;
    PaStream           *stream;
    PaError             err = paNoError;
    rbComm              data = {0};
    unsigned            delayCntr;
    unsigned            numSamples;
    unsigned            numBytes;

    // Set the ring buffer size to about 500 ms
    // We can adjust this after testing                ### this value
    numSamples = NextPowerOf2((unsigned)(SAMPLE_RATE * 0.5 * NUM_CHANNELS));
    numBytes = numSamples * sizeof(SAMPLE);
    data.ringBufferData = (SAMPLE *) PaUtil_AllocateMemory(numBytes);

    // Make sure ring buffer was allocated
    if (data.ringBufferData == NULL)
    {
        fprintf(stderr, "%sCould not allocate ring buffer data of size %d.\n", HL_ERROR_PREFIX, numBytes);
        goto done;
    }

    if (PaUtil_InitializeRingBuffer(&data.ringBuffer, sizeof(SAMPLE), numSamples, data.ringBufferData) < 0)
    {
        fprintf(stderr, "%sFailed to initialize ring buffer. Perhaps size is not power of 2?\nSize: %d\n", HL_ERROR_PREFIX, numSamples);
        goto done;
    }

    // TODO: Grab HulaLoop driver specifically/make this grabbable for RECORD devices
    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice)
    {
        fprintf(stderr, "%sNo default input device found.\n", HL_ERROR_PREFIX);
        goto done;
    }

    // Setup the stream for the selected device
    inputParameters.channelCount = Pa_GetDeviceInfo(inputParameters.device)->maxInputChannels;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  // &outputParameters
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      // we won't output out of range samples so don't bother clipping them
              recordCallback,
              &data);

    PA_CHECK_ERROR

    // Start a handler to read from the ring buffer
    err = startThread(&data, &testWrite);
    PA_CHECK_ERROR

    // Start the stream
    err = Pa_StartStream(stream);
    PA_CHECK_ERROR

    delayCntr = 0;
    while ( delayCntr++ < NUM_SECONDS )
    {
        printf("rb write index = 0x%d\n", data.ringBuffer.writeIndex);
        fflush(stdout);
        Pa_Sleep(1000);
    }
    PA_CHECK_ERROR

    err = Pa_CloseStream(stream);
    PA_CHECK_ERROR

    // Stop the thread
    err = stopThread(&data);
    PA_CHECK_ERROR

done:
    // Make sure this is still valid before freeing
    if (data.ringBufferData)
    {
        PaUtil_FreeMemory(data.ringBufferData);
    }

    if (err != paNoError)
    {
        fprintf(stderr, "%sAn error occured while using the PortAudio stream\n", HL_ERROR_PREFIX);
        fprintf(stderr, "%sError number: %d\n", HL_ERROR_PREFIX, err);
        fprintf(stderr, "%sError message: %s\n", HL_ERROR_PREFIX, Pa_GetErrorText(err));
        err = 1;
    }
}

/**
 * Utility function for fetching the PortAudio devices of a given DeviceType.
 *
 */
vector<Device *> OSXAudio::getDevices(DeviceType type)
{
    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0)
    {
        fprintf(stderr, "%sNo PortAudio devices were found.\nError code: 0x%x\n", HL_ERROR_PREFIX, deviceCount);
        exit(1); // TODO: Handle error
    }

    vector<Device *> devices;
    for (uint32_t i = 0; i < deviceCount; i++)
    {
        const PaDeviceInfo *paDevice = Pa_GetDeviceInfo(i);

        // Exclude record only devices if type == PLAYBACK
        if (type == DeviceType::PLAYBACK && paDevice->maxOutputChannels == 0)
        {
            continue;
        }

        // Create HulaLoop style device and add to vector
        // This needs to be freed elsewhere
        Device *hlDevice = new Device(NULL, string(paDevice->name), type);
        devices.push_back(hlDevice);

        // Print some debug device info for now
        // TODO: Remove
        cout << "Device #" << i + 1 << ": " << paDevice->name << endl;
        cout << "Input Channels: " << paDevice->maxInputChannels << endl;
        cout << "Output Channels: " << paDevice->maxOutputChannels << endl;
        cout << "Default Sample Rate: " << paDevice->defaultSampleRate << endl;
        cout << endl;
    }

    return devices;
}

/**
 * Fetch all PortAudio devices that are eligible for recording.
 * In our case, this includes all output devices since we want to do loopback.
 */
vector<Device *> OSXAudio::getInputDevices()
{
    return getDevices(DeviceType::RECORD);
}

/**
 * Fetch all PortAudio devices that are eligible for playback.
 *
 */
vector<Device *> OSXAudio::getOutputDevices()
{
    return getDevices(DeviceType::PLAYBACK);
}

/**
 * Static function in the current instance of the class
 * to allow thread execution
 *
 * @param _this Instance of the current object
 */
void OSXAudio::test_capture(OSAudio *_this)
{
    // Sleep for a few seconds just to keep the logs clean
    Pa_Sleep(2000);

    _this->capture();
}

/**
 * Set the selected output device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
void OSXAudio::setActiveOutputDevice(Device *device)
{

}

/**
 * Deconstructs the OSXAudio instance
 */
OSXAudio::~OSXAudio()
{

}