#include <iostream>
#include <stdlib.h>

#include "OSAudio.h"
#include "WindowsAudio.h"

#include "Device.h"

int main()
{
    printf("Hello world!\n");

    OSAudio* audio = new WindowsAudio();

    vector<Device*> deviceList = audio->getOutputDevices();

    printf("Count: %d\n", deviceList.size());

    for(int i = 0;i < deviceList.size();i++)
    {
        printf("Device %d : %d\n", i, deviceList[i]->getID());
    }
}