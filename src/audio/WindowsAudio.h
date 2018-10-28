#ifndef WIN_AUDIO
#define WIN_AUDIO

// Windows Audio
#include <Audioclient.h>
#include <comdef.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <windows.h>

// Do not move this before mmdeviceapi.h
#include <functiondiscoverykeys_devpkey.h>

// PortAudio
#include <portaudio.h>

// System
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "hlaudio/internal/Device.h"
#include "hlaudio/internal/OSAudio.h"

using namespace std;

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

// Error handling
#define HANDLE_ERROR(hres) \
            if (FAILED(hres)) { goto Exit; }
#define HANDLE_PA_ERROR(hres) \
            if (hres != paNoError) { goto Exit; }
#define SAFE_RELEASE(punk) \
            if ((punk) != NULL) \
                { (punk)->Release(); (punk) = NULL; }

/**
 * A audio class that captures system wide audio on Windows
 */
class WindowsAudio : public OSAudio {
    private:
        const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
        const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
        const IID IID_IAudioClient = __uuidof(IAudioClient);
        const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

        // System necessary variables
        HRESULT status;
        PaError pa_status;

        REFERENCE_TIME requestDuration = REFTIMES_PER_SEC;
        REFERENCE_TIME bufferDuration;

        IMMDeviceEnumerator *pEnumerator = NULL;
        IMMDeviceCollection *deviceCollection = NULL;

        // Audio data
        uint8_t *pData;

    public:
        WindowsAudio();
        ~WindowsAudio();

        bool checkRates(Device *device);
        vector<Device *> getDevices(DeviceType type);

        void capture();

        void setActiveOutputDevice(Device *device);
};

#endif