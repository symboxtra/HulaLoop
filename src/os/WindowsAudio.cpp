#include "WindowsAudio.h"

WindowsAudio::WindowsAudio()
{
    thread captureThread(capture);
}

vector<Device*> WindowsAudio::getInputDevices()
{
    vector<Device*> temp;

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
    vector<Device*> temp;

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

    if(FAILED(status))
        return NULL;
    else
        return temp;
}

void WindowsAudio::capture()
{
    IAudioCaptureClient *captureClient = NULL;
    IAudioClient* audioClient = NULL;
    WAVEFORMATEX* pwfx = NULL;
    IMMDevice* audioDevice = NULL;

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

    status = audioClient->GetService(IID_IAudioCaptureClient, (void**)*captureClient);
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
                    thread{callbackList[i], numFramesAvailable, pData}.detach();
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