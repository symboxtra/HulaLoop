#include "WindowsAudio.h"

WindowsAudio::WindowsAudio()
{
    //thread captureThread(&WindowsAudio::capture, this);
}

vector<Device*> WindowsAudio::getInputDevices()
{
    vector<Device*> temp;

    uint16_t deviceCount = waveInGetNumDevs();
    if(deviceCount > 0)
    {
        for(int i = 0;i < deviceCount;i++)
        {
            WAVEINCAPSW waveInCaps;
            waveInGetDevCapsW(i, &waveInCaps, sizeof(WAVEINCAPS));

            //temp.push_back(new Device()); //TODO: Figure out Device Class
        }
    }

    return temp;
}

vector<Device*> WindowsAudio::getOutputDevices()
{
    
    vector<Device*> temp;
    IMMDevice* pp;

    status = CoInitialize(NULL);
    HANDLE_ERROR(status);

    // Create instance? don't exactly know what this does yet
    status = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
    HANDLE_ERROR(status);

    // Get Device Collection
    status = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &deviceCollection);
    HANDLE_ERROR(status);

    // Store all devices as Device instances into vector
    UINT count = 0;
    status = deviceCollection->GetCount(&count);
    HANDLE_ERROR(status);
    
    for(int i = 0;i < count;i++)
    {
        
        IMMDevice* device;
        DWORD state = NULL;

        status = deviceCollection->Item(i, &device);
        HANDLE_ERROR(status);


        status = device->GetState(&state);
        HANDLE_ERROR(status);

        LPWSTR id = NULL;
        status = device->GetId(&id);
        HANDLE_ERROR(status);

        // Add device to devicelist
        IPropertyStore* propKey;
        PROPVARIANT varName;
        PropVariantInit(&varName);

        status = device->OpenPropertyStore(STGM_READ, &propKey);
        HANDLE_ERROR(status);
        status = propKey->GetValue(PKEY_Device_FriendlyName, &varName);
        HANDLE_ERROR(status);

        char* buffer = (char*)malloc(500);
        wcstombs(buffer, varName.pwszVal, 500);

        Device* audio = new Device((uint32_t)id, buffer);
        temp.push_back(audio);
        
        bool flag = true;
        for(int j = 0;j < deviceList.size();j++)
        {
            if(deviceList[j]->getID() == (uint32_t)id)
                flag = false;
        }

        if(flag)
            deviceList.push_back(audio);
    }

Exit:
    SAFE_RELEASE(pEnumerator)
    //SAFE_RELEASE(pp)

    if(FAILED(status))
    {
        _com_error err(status);
        LPCTSTR errMsg = err.ErrorMessage();
        printf("\nError: %s\n", errMsg);
        return {};
    }
    else
        return temp;
}

void WindowsAudio::test_capture(WindowsAudio* _this) { _this->capture(); }

void WindowsAudio::capture()
{
    IAudioCaptureClient* captureClient = NULL;
    IAudioClient* audioClient = NULL;
    WAVEFORMATEX* pwfx = NULL;
    IMMDevice* audioDevice = NULL;
    UINT32 numFramesAvailable;
    DWORD flags;

    uint32_t packetLength = 0;

    REFERENCE_TIME duration;

    status = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
    HANDLE_ERROR(status);

    status = pEnumerator->GetDevice((LPCWSTR)(activeOutputDevice->getID()), &audioDevice);
    HANDLE_ERROR(status);

    status = audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&audioClient);
    HANDLE_ERROR(status);

    status = audioClient->GetMixFormat(&pwfx);
    HANDLE_ERROR(status);

    status = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, REFTIMES_PER_SEC, 0, pwfx, NULL);
    HANDLE_ERROR(status);

    status = audioClient->GetService(IID_IAudioCaptureClient, (void**)&captureClient);
    HANDLE_ERROR(status);

    status = audioClient->Start();
    HANDLE_ERROR(status);

    duration = (double)REFTIMES_PER_SEC * captureBufferSize / pwfx->nSamplesPerSec;

    // Continue loop under process ends
    while(true)
    {
        while(callbackList.size() > 0)
        {
            Sleep(duration / (REFTIMES_PER_MILLISEC * 2));

            status = captureClient->GetNextPacketSize(&packetLength);
            HANDLE_ERROR(status);

            while (packetLength != 0)
            {
                status = captureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
                HANDLE_ERROR(status);

                if(flags & AUDCLNT_BUFFERFLAGS_SILENT)
                    pData = NULL;
                
                // Execute callbacks
                for(int i = 0;i < callbackList.size();i++)
                {
                    //thread{callbackList[i], numFramesAvailable, pData}.detach();
                }

                status = captureClient->ReleaseBuffer(numFramesAvailable);
                HANDLE_ERROR(status);

                status = captureClient->GetNextPacketSize(&packetLength);
                HANDLE_ERROR(status);
            }
        }

        status = audioClient->Stop();
        HANDLE_ERROR(status);
    }

Exit:
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pEnumerator);
    SAFE_RELEASE(audioDevice);
    SAFE_RELEASE(audioClient);
    SAFE_RELEASE(captureClient);

    //TODO: Handle error accordingly
}