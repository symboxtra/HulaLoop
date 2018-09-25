#include "WindowsAudio.h"

WindowsAudio::WindowsAudio()
{
    //thread captureThread(&WindowsAudio::capture, this);
    DWORD threadID;
    
    int a = 4;
    status = CoInitialize(NULL);
    _com_error err(status);
    LPCTSTR errMsg = err.ErrorMessage();
    cout << "\nError: " << errMsg << "\n" << endl;
    
    HANDLE myHandle = CreateThread(0, 0, WindowsAudio::test_capture, this, 0, &threadID);
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

            //Device* audio = new Device(waveInCaps.wPid, str, DeviceType::RECORDING);
            //temp2.push_back(audio);
            //deviceList.push_back(audio);
        }
    }

    return temp2;
}

vector<Device*> WindowsAudio::getOutputDevices()
{
    
    vector<Device*> temp;
    IMMDevice* pp;

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

        //char* buffer = (char*)malloc(500);
        //wcstombs(buffer, varName.pwszVal, 500);

        wstring fun(varName.pwszVal);
        string str(fun.begin(), fun.end());

        Device* audio = new Device(id, str, DeviceType::PLAYBACK);
        temp.push_back(audio);
        
        bool flag = true;
        for(int j = 0;j < deviceList.size();j++)
        {
            if(deviceList[j]->getID() == id)
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

DWORD __stdcall WindowsAudio::test_capture(LPVOID param)
{
    cout << "Hello" << endl;
    WindowsAudio* winAud = reinterpret_cast<WindowsAudio*>(param);
    winAud->capture();

    return 0;
}

void WindowsAudio::capture()
{
    cout << "Temp hello" << endl;
    IAudioCaptureClient* captureClient = NULL;
    IAudioClient* audioClient = NULL;
    WAVEFORMATEX* pwfx = NULL;
    IMMDevice* audioDevice = NULL;
    UINT32 numFramesAvailable;
    DWORD flags;

    uint32_t packetLength = 0;

    REFERENCE_TIME duration;

    cout << "Test I'm here!" << endl;
    status = CoInitialize(NULL);
    HANDLE_ERROR(status);
    

    status = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
    HANDLE_ERROR(status);
    cout << "Test I'm here!" << endl;

    cout << "Test I1'm here!" << endl;
    //status = pEnumerator->GetDevice(activeOutputDevice->getID(), &audioDevice);
    status = pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &audioDevice);
    cout << "Test I'm here1!" << endl;
    HANDLE_ERROR(status);
    cout << "Test I'm here!" << endl;

    status = audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&audioClient);
    HANDLE_ERROR(status);
    cout << "Test I'm here00!" << endl;

    status = audioClient->GetMixFormat(&pwfx);
    HANDLE_ERROR(status);
    cout << pwfx << endl;

    //status = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, REFTIMES_PER_SEC, 0, pwfx, NULL);
    REFERENCE_TIME req = REFTIMES_PER_SEC;
    status = audioClient->Initialize(
                         AUDCLNT_SHAREMODE_SHARED,
                         0,
                         req,
                         0,
                         pwfx,
                         NULL);
    HANDLE_ERROR(status);
    cout << "Test I'm here1!" << endl;

    status = audioClient->GetService(IID_IAudioCaptureClient, (void**)&captureClient);
    HANDLE_ERROR(status);
    cout << "Test I'm here2!" << endl;

    status = audioClient->Start();
    HANDLE_ERROR(status);
    cout << "Test I'm here3!" << endl;

    duration = (double)REFTIMES_PER_SEC * captureBufferSize / pwfx->nSamplesPerSec;

    // Continue loop under process ends
    return;
    while(true)
    {
        cout << "\n\nAm I on?\n\n" << endl;
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

    _com_error err(status);
    LPCTSTR errMsg = err.ErrorMessage();
    cout << "\nError: " << errMsg << "\n" << endl;

    //TODO: Handle error accordingly
}