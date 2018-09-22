#include <iostream>
#include <stdlib.h>

#include "OSAudio.h"
#include "WindowsAudio.h"

#include "Device.h"

int main()
{
    OSAudio* audio = new WindowsAudio();

    vector<Device*> deviceList = audio->getOutputDevices();

    printf("** Audio Device List **\nCount: %d\n", deviceList.size());
    for(int i = 0;i < deviceList.size();i++)
        printf("Device %d : ID = %d\tNAME = %s\n", i, deviceList[i]->getID(), deviceList[i]->getName());
}