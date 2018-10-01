#include <iostream>
#include <stdlib.h>
#include <iostream>

#include "Controller.h"

#include "Device.h"

int main()
{
    //OSAudio* audio = new WindowsAudio();

    Controller* cp = new Controller();

    //vector<Device*> deviceList = audio->getOutputDevices();
    //audio->getInputDevices();
    //deviceList = audio->getAudioDevices();

    //cout << "** Audio Device List **\tCount: " << deviceList.size() << endl;
    //for(int i = 0;i < deviceList.size();i++)
      //  cout << "Device " << i << " : ID = " << deviceList[i]->getID() << "\tNAME = " << deviceList[i]->getName() << "\t TYPE = " << int(deviceList[i]->getType()) << endl;
    while(true);
}