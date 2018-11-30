/**
 * @file Simple.cpp
 * Simple, single-threaded example using the HulaLoop audio library.
 *
 * The program retrieves a device list, displays the devices,
 * allows the user to choose a device, and then prints the number of
 * samples read from that device.
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

    cout << endl << "-------- HulaLoop Simple Example --------" << endl << endl;

    // Fetch all loopback and recording devices
    // Loopback devices are output devices whose output stream can be "looped back" and recorded
    // Recording devices are actual inputs like microphones
    vector<Device *> devices = c.getDevices((DeviceType)(DeviceType::LOOPBACK | DeviceType::RECORD));

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

    // Choose the device which we're going to get audio from
    c.setActiveInputDevice(devices[choice]);

    // Devices should always be deleted as soon as they are not needed
    Device::deleteDevices(devices);

    // Create a buffer to hold our audio
    // This buffer must be continuously read from so that it
    // does not overflow
    HulaRingBuffer rb(2);

    // Let the controller know that we want to receive audio
    c.addBuffer(&rb);

    // Print the number of samples we read from the buffer
    // Read a maximum of 1024 samples
    ring_buffer_size_t maxSamples = 1024;
    float samples[1024];
    while (true)
    {
        ring_buffer_size_t samplesRead =  rb.read((float *)&samples, maxSamples);

        // Print the number of samples read
        if (samplesRead > 0)
        {
            cout << "Read: " << samplesRead << " samples." << endl;
        }
    }
}