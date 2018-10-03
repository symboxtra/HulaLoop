#include <cstdint>
#include <iostream>
#include <portaudio.h>

#include "OSXAudio.h"
#include "OSXDaemon/OSXDaemon.h"

/**
 * Constructs an instance of OSXAudio class.
 *
 * This will initialize an OSXDaemon which will handle the virtual loopback.
 * OSXDaemon will join or start a JACK server to get audio.
 */
OSXAudio::OSXAudio()
{
    // Create the loopback daemon
    OSXDaemon * osxDaemon;
    osxDaemon = new OSXDaemon("HulaLoop #1", 0);
    osxDaemon->activate();

    getInputDevices();
}

/**
 * Execution loop for loopback capture
 */
void OSXAudio::capture()
{

}

/**
 * Utility function for fetching the PortAudio devices of a given DeviceType.
 *
 */
vector<Device *> OSXAudio::getDevices(DeviceType type)
{
    int ret = Pa_Initialize();
    if(ret != paNoError)
    {
        fprintf(stderr, "%sPortAudio failed to initialize.\nError code: 0x%x\n", HL_ERROR_PREFIX, ret);
        exit(1);
    }

    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0)
    {
        fprintf(stderr, "%sNo PortAudio devices were found.\nError code: 0x%x\n", HL_ERROR_PREFIX, deviceCount);
        exit(1);
    }

    vector<Device *> devices;
    for(uint32_t i = 0; i < deviceCount; i++)
    {
        const PaDeviceInfo * paDevice = Pa_GetDeviceInfo(i);

        // Exclude record only devices if type == PLAYBACK
        if (type == DeviceType::PLAYBACK && paDevice->maxOutputChannels == 0)
            continue;

        // Create HulaLoop style device and add to vector
        // This needs to be freed elsewhere
        Device * hlDevice = new Device(NULL, string(paDevice->name), type);
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
    return getDevices(DeviceType::RECORDING);
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
void OSXAudio::test_capture(OSXAudio * param)
{

}

/**
 * Set the selected output device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
void OSXAudio::setActiveOutputDevice(Device * device)
{

}

/**
 * Deconstructs the OSXAudio instance
 */
OSXAudio::~OSXAudio()
{

}
