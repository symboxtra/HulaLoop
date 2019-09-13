/**
 * @file SimplePlayback.cpp
 * Simple, single-threaded example that uses the HulaLoop audio library
 * to playback audio.
 *
 * The program retrieves a device list, displays the devices,
 * allows the user to choose a device, and then plays the raw audio file on the
 * selected device
 */

#include <hlaudio/hlaudio.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace hula;

int main()
{
    // Create the controller
    // This is the hub for all communication and control
    Controller c;

    cout << endl << "-------- HulaLoop Simple Playback Example --------" << endl << endl;

    // Fetch all the playback devices
    // "Playback" devices entail the audio devices that have the ability to play audio data
    vector<Device *> devices = c.getDevices((DeviceType)(DeviceType::PLAYBACK));

    for (int i = 0; i < devices.size(); i++)
    {
        cout << "Device #" << i << ": " << devices[i]->getName() << endl;
    }

    cout << "Enter a device ID: ";

    int choice = 0;
    cin >> choice;

    if (choice > devices.size())
    {
        cout << "ID out of range. Please try again." << endl;
        exit(1);
    }

    cout << "Enter name of raw audio file: ";

    string fileName;
    cin >> fileName;

    FILE *file = fopen(fileName.c_str(), "rb");
    if(!file)
    {
        cout << "Invalid filepath. Please try again." << endl;
        exit(1);
    }

    // Choose the device which we're going to play audio on
    c.setActiveOutputDevice(devices[choice]);

    // Devices should always be deleted as soon as they are not needed
    Device::deleteDevices(devices);

    // Let the controller know to start the playback threads
    c.startPlayback();

    // Print the number of samples we read from the buffer
    // Read a maximum of 1024 samples
    ring_buffer_size_t maxSamples = 1024;
    float samples[1024];
    while (true)
    {
        size_t numRead = fread(samples, sizeof(float), maxSamples, file);
        c.copyToBuffers(samples, maxSamples * sizeof(float));

        if(numRead < maxSamples)
        {
            cout << "Playback finished" << endl;
            break;
        }
    }
    c.endPlayback();
}