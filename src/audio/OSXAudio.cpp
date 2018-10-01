#include <cstdint>
#include <iostream>
#include <portaudio.h>

#include "OSXAudio.h"

OSXAudio::OSXAudio()
{
    getInputDevices();

}

void OSXAudio::capture()
{

}

/**
 * \brief Utility function for fetching the PortAudio devices of a given DeviceType.
 *
 */
vector<Device *> OSXAudio::getDevices(DeviceType type)
{
    int ret = Pa_Initialize();
    if(ret != paNoError)
    {
        fprintf(stderr, "Error: PortAudio failed to initialize.\nError code: 0x%x\n", ret);
        exit(1);
    }

    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0)
    {
        fprintf(stderr, "Error: No PortAudio devices were found.\nError code: 0x%x\n", deviceCount);
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
 * \brief Fetch all PortAudio devices that are eligible for recording.
 * In our case, this includes all output devices since we want to do loopback.
 */
vector<Device *> OSXAudio::getInputDevices()
{
    return getDevices(DeviceType::RECORDING);
}

/**
 * \brief Fetch all PortAudio devices that are eligible for playback.
 *
 */
vector<Device *> OSXAudio::getOutputDevices()
{
    return getDevices(DeviceType::PLAYBACK);
}

void OSXAudio::test_capture(OSXAudio * param)
{

}

void OSXAudio::setActiveOutputDevice(Device * device)
{

}

OSXAudio::~OSXAudio()
{

}
