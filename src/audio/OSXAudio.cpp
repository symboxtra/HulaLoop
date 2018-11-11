#include <portaudio.h>

#include <cstdint>
#include <thread>

#include <QDebug>

#include "OSXAudio.h"
#include "hlaudio/internal/HulaAudioError.h"
#include "hlaudio/internal/HulaAudioSettings.h"

using namespace hula;

/**
 * Constructs an instance of OSXAudio class.
 *
 * This will initialize an OSXDaemon which will handle the virtual loopback.
 * OSXDaemon will join or start a JACK server to get audio.
 */
OSXAudio::OSXAudio()
{
    // Create the loopback daemon
    osxDaemon = new OSXDaemon("HulaLoop #1", 0); // TODO: Make sure this can handle multiple instances of HulaLoop running
    osxDaemon->activate();
    osxDaemon->monitor();

    // TODO: qOut()
    qInfo("\n\n");

    // Initialize PortAudio
    int err = Pa_Initialize();
    if (err != paNoError)
    {
        qCritical("PortAudio failed to initialize.");
        qCritical("PortAudio: %s", Pa_GetErrorText(err));
        exit(1); // TODO: Handle error
    }

    // TODO: qOut()
    qInfo("\n\n");
}

/**
 * This routine will be called by the PortAudio engine when audio is needed.
 * It may be called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
 */
static int paRecordCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{
    OSXAudio *obj = (OSXAudio *)userData;

    // Prevent unused variable warnings.
    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    // TODO: Make sure this calculation is right
    obj->copyToBuffers(inputBuffer, framesPerBuffer * NUM_CHANNELS * sizeof(SAMPLE));

    return paContinue;
}

/**
 * Execution loop for loopback capture
 *
 * Based on code from https://app.assembla.com/spaces/portaudio/git/source/master/examples/paex_record_file.c
 */
void OSXAudio::capture()
{
    PaStreamParameters  inputParameters = {0};
    PaStream           *stream;
    PaError             err = paNoError;

    inputParameters.device = *(this->activeInputDevice->getID());
    if (inputParameters.device == paNoDevice)
    {
        qWarning() << "Device %s not found." << QString::fromStdString(this->activeInputDevice->getName());
        exit(1); // TODO: Handle error
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
              paClipOff,             // We won't output out of range samples so don't bother clipping them
              paRecordCallback,
              this                   // Pass our instance in
          );

    if (err != paNoError)
    {
        qCritical("Could not open Port Audio device stream.");
        qCritical("PortAudio: %s", Pa_GetErrorText(err));
        exit(1); // TODO: Handle error
    }

    // Start the stream
    err = Pa_StartStream(stream);
    if (err != paNoError)
    {
        qCritical("Could not start Port Audio device stream.");
        qCritical("PortAudio: %s", Pa_GetErrorText(err));
        exit(1); // TODO: Handle error
    }

    while (!this->endCapture)
    {
        // Keep this thread alive
        // The second half of this function could be moved to a separate
        // function like endCapture() so that we don't have to keep this thread alive.

        qDebug("Capture keep-alive");

        // This value can be adjusted
        // 100 msec is decent precision for now
        Pa_Sleep(1000);
    }

    qDebug("Capture thread ended keep-alive.\n");

    if (err != paNoError)
    {
        qCritical("Error during read from device stream.");
        qCritical("PortAudio: %s", Pa_GetErrorText(err));
        exit(1); // TODO: Handle error
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError)
    {
        qCritical("Could not close Port Audio device stream.");
        qCritical("PortAudio: %s", Pa_GetErrorText(err));
        exit(1); // TODO: Handle error
    }
}

/**
 * Fetch devices of the specified type.
 * These devices must be deleted by the caller using the
 * Device::deleteDevices() method.
 *
 * The list is designed for one-time use. Get the list,
 * use the device, delete the list immediately.
 *
 * DO NOT STORE THIS as it may become out-of-date.
 *
 * @return List of Device objects
 */
std::vector<Device *> OSXAudio::getDevices(DeviceType type)
{
    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0)
    {
        qWarning("Failed to fetch PortAudio devices.");
        exit(1); // TODO: Handle error
    }

    std::vector<Device *> devices;
    for (uint32_t i = 0; i < deviceCount; i++)
    {
        const PaDeviceInfo *paDevice = Pa_GetDeviceInfo(i);
        DeviceType checkType = (DeviceType)0;

        // We can only support OSX loopback on our own driver
        if (strcmp(paDevice->name, "HulaLoop") == 0)
        {
            checkType = (DeviceType)(checkType | LOOPBACK);
        }
        if (paDevice->maxOutputChannels > 0)
        {
            checkType = (DeviceType)(checkType | PLAYBACK);
        }
        if (paDevice->maxInputChannels > 0)
        {
            checkType = (DeviceType)(checkType | RECORD);
        }

        // Create HulaLoop style device and add to vector
        // This needs to be freed elsewhere
        if (type & checkType)
        {
            Device *hlDevice = new Device(new uint32_t(i), std::string(paDevice->name), checkType);
            devices.push_back(hlDevice);
        }
    }

    return devices;
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
 * Checks the sampling rate and bit depth of the device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
bool OSXAudio::checkRates(Device *device)
{
    PaStreamParameters inputParameters = {0};
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.device = *device->getID();
    inputParameters.sampleFormat = paFloat32;

    PaError err = Pa_IsFormatSupported(&inputParameters, NULL, HulaAudioSettings::getInstance()->getSampleRate());

    if (err == paFormatIsSupported)
    {
        // TODO: qOut()
        qInfo() << tr(HL_SAMPLE_RATE_VALID);
    }
    else
    {
        // TODO: qOut()
        qInfo() << tr(HL_SAMPLE_RATE_INVALID);
    }

    return err == paFormatIsSupported;
}

/**
 * Deconstructs the OSXAudio instance
 */
OSXAudio::~OSXAudio()
{
    qDebug("OSXAudio destructor called");

    // Close the Port Audio session
    PaError err = Pa_Terminate();
    if (err != paNoError)
    {
        qCritical("Could not terminate Port Audio session.");
        qCritical("PortAudio: %s", Pa_GetErrorText(err));
    }

    // Stop the daemon
    if (osxDaemon)
    {
        delete osxDaemon;
    }
}
