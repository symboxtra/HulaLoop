#include "WindowsAudio.h"

WindowsAudio::WindowsAudio()
{

}

void WindowsAudio::setBufferSize(uint32_t size)
{
    this->captureBufferSize = size;
}

void WindowsAudio::addBufferReadyCallback(f_int_t callFunction)
{
    this->callbackList.push_back(callFunction);
}

vector<Device*> WindowsAudio::getAudioDevices()
{
    deviceList = NULL;

    vector<Device*> temp = getInputDevices();
    if(temp != NULL)
        deviceList.insert(deviceList.end(), temp.begin(), temp.end());
    
    temp = getOutputDevices();
    if(temp != NULL)
        deviceList.insert(deviceList.end(), temp.begin(), temp.end());

    return deviceList;
}

vector<Device*> WindowsAudio::getInputDevices()
{
    vector<Device*> temp = NULL;

    uint deviceCount = waveInGetNumDevs();
    if(deviceCount > 0)
    {
        for(int i = 0;i < deviceCount;i++)
        {
            WAVEINCAPSW waveInCaps;
            waveInGetDevCapsW(i, &waveInCaps, sizeof(WAVEINCAPS));

            temp.push_back(new Device()); //TODO: Figure out Device Class
        }
    }

    return temp;
}

vector<Device*> WindowsAudio::getOutputDevices()
{
    vector<Device*> temp = NULL;

    // Create instance? don't exactly know what this does yet
    status = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
    HANDLE_ERROR(status);

    // Get Device Collection
    status = pEnumerator->EnumAudioEndpoints(eCapture, eConsole, &deviceCollection);
    HANDLE_ERROR(status);

    // Store all devices as Device instances into vector
    int count = 0;
    status = deviceCollection->GetCount(&count);
    HANDLE_ERROR(status);

    IMMDevice* pp = NULL;
    for(int i = 0;i < count;i++)
    {
        pp = NULL;
        DWORD state = NULL;

        status = deviceCollection->Item(i, &pp);
        HANDLE_ERROR(status);

        status = pp->GetState(&state);
        HANDLE_ERROR(status);

        if(state == DEVICE_STATE_ACTIVE)
            // Add device to devicelist
            temp.push_back(new Device()) //TODO: Figure out Device Class
    }

Exit:
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pp)

    if(FAILED(hres))
        return NULL;
    else
        return temp;
}

void WindowsAudio::setActiveOutputDevice(Device* device)
{
    this->activeOutputDevice = device;
}

void WindowsAudio::setActiveRecordDevice(Device* device)
{
    this->activeInputDevice = device;
}