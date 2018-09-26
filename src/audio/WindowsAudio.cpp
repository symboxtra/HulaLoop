#include "WindowsAudio.h"

WindowsAudio::WindowsAudio()
{
    promise<void> e1;
    thread captureThread(test_capture, this, move(e1.get_future()));

    // Temporarily joining thread so that app continues
    captureThread.join();
    // Handle error if unacceptable result

}

vector<Device*> WindowsAudio::getInputDevices()
{
    vector<Device*> temp2;

    uint16_t deviceCount = waveInGetNumDevs();
    if(deviceCount > 0)
    {
        for(int i = 0;i < deviceCount;i++)
        {
            WAVEINCAPSW waveInCaps;    
            waveInGetDevCapsW( i, &waveInCaps, sizeof( WAVEINCAPSW ) );
            wstring temp(waveInCaps.szPname);   
            string str(temp.begin(), temp.end());
        }
    }

    return temp2;
}

vector<Device*> WindowsAudio::getOutputDevices()
{
    
    vector<Device*> temp;

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
    
    for(UINT i = 0;i < count;i++)
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

        //char* buffer = (char*)malloc(500);
        //wcstombs(buffer, varName.pwszVal, 500);

        wstring fun(varName.pwszVal);
        string str(fun.begin(), fun.end());

        Device* audio = new Device(reinterpret_cast<uint32_t*>(id), str, DeviceType::PLAYBACK);
        temp.push_back(audio);
        
        bool flag = true;
        for(int j = 0;j < deviceList.size();j++)
        {
            if(reinterpret_cast<LPCWSTR>(deviceList[j]->getID()) == id)
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

void WindowsAudio::setActiveOutputDevice(Device* device)
{
    // this->activeOutputDevice = device;

    // // Interrupt all threads and make sure they stop 
    // for(auto& t : execThreads)
    // {
    //     t.second.set_value();
    //     t.first.join();
    // }

    // execThreads.clear();
    // // Start up new threads with new selected device info
    // promise<void> e1;
    // //thread t1(&WindowsAudio::test_capture, this, move(e1.get_future()));
    // //execThreads.push_back(make_pair(t1, e1));

    // // Add playback thread later
}

void WindowsAudio::test_capture(WindowsAudio* param, future<void> futureObj)
{
    cout << "Hello" << endl;
    param->capture(move(futureObj));
}

/**
 * Execution loop for loopback capture
 */
void WindowsAudio::capture(future<void> futureObj)
{
    cout << "Temp hello" << endl;
    
    IAudioCaptureClient* captureClient = NULL;
    IAudioClient* audioClient = NULL;
    WAVEFORMATEX* pwfx = NULL;
    IMMDevice* audioDevice = NULL;
    UINT32 numFramesAvailable;
    DWORD flags;
    char* buffer = (char*)malloc(500);


    uint32_t packetLength = 0;

    REFERENCE_TIME duration;

    cout << "Test I'm here!" << endl;
    status = CoInitialize(NULL);
    HANDLE_ERROR(status);
    

    status = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
    HANDLE_ERROR(status);
    cout << "Test I'm here!" << endl;

    cout << "Test I1'm here!" << endl;
    //status = pEnumerator->GetDevice(reinterpret_cast<LPCWSTR>(activeOutputDevice->getID()), &audioDevice);
    status = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audioDevice);
    // Add device to devicelist
        IPropertyStore* propKey;
        PROPVARIANT varName;
        PropVariantInit(&varName);

        status = audioDevice->OpenPropertyStore(STGM_READ, &propKey);
        status = propKey->GetValue(PKEY_Device_FriendlyName, &varName);
        wcstombs(buffer, varName.pwszVal, 500);
        cout << buffer << endl;
    
    cout << "Test I'm here1!" << endl;
    HANDLE_ERROR(status);
    cout << "Test I'm here!" << endl;

    status = audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&audioClient);
    HANDLE_ERROR(status);
    cout << "Test I'm here00!" << endl;
    
    
    status = audioClient->GetMixFormat(&pwfx);
    HANDLE_ERROR(status);

    REFERENCE_TIME req = REFTIMES_PER_SEC;
    status = audioClient->Initialize(
                         AUDCLNT_SHAREMODE_SHARED,
                         AUDCLNT_STREAMFLAGS_LOOPBACK,
                         req,
                         0,
                         pwfx,
                         NULL);
    HANDLE_ERROR(status);
    cout << "Test I'm here1!" << endl;

    status = audioClient->GetBufferSize(&captureBufferSize);
    HANDLE_ERROR(status)

    status = audioClient->GetService(IID_IAudioCaptureClient, (void**)&captureClient);
    HANDLE_ERROR(status);
    cout << "Test I'm here2!" << endl;

    status = audioClient->Start();
    HANDLE_ERROR(status);
    cout << "Test I'm here3!" << endl;

    duration = (double)REFTIMES_PER_SEC * captureBufferSize / pwfx->nSamplesPerSec;

    // Continue loop under process ends
    while(futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        //cout << "\n\nAm I on?\n\n" << endl;
       // while(callbackList.size() > 0)
       // {
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
                    //thread callback(callbackList[i], numFramesAvailable, pData);
                    //callback.detach();

                    // TODO: Make these calls asynchronous so it does not delay run process
                    callbackList[i](numFramesAvailable, pData);
                }

                status = captureClient->ReleaseBuffer(numFramesAvailable);
                HANDLE_ERROR(status);

                status = captureClient->GetNextPacketSize(&packetLength);
                HANDLE_ERROR(status);
            }
       // }
    }

    status = audioClient->Stop();
    HANDLE_ERROR(status);

Exit:
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pEnumerator);
    SAFE_RELEASE(audioDevice);
    SAFE_RELEASE(audioClient);
    SAFE_RELEASE(captureClient);

    _com_error err(status);
    LPCTSTR errMsg = err.ErrorMessage();
    cout << "\nError: " << errMsg << "\n" << endl;

    //TODO: Handle error accordingly
    
}