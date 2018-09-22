#include <iostream>
#include <stdlib.h>
#include <iostream>

#include "OSAudio.h"
#include "WindowsAudio.h"

#include "Device.h"

int main()
{
    OSAudio* audio = new WindowsAudio();

    vector<Device*> deviceList = audio->getOutputDevices();
    audio->getInputDevices();
    deviceList = audio->getAudioDevices();

    cout << "** Audio Device List **\tCount: " << deviceList.size() << endl;
    for(int i = 0;i < deviceList.size();i++)
        cout << "Device " << i << " : ID = " << deviceList[i]->getID() << "\tNAME = " << deviceList[i]->getName() << "\t TYPE = " << int(deviceList[i]->getType()) << endl;
}