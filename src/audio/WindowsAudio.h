#ifndef WIN_AUDIO
#define WIN_AUDIO

// System
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// Windows Audio
#include <Audioclient.h>
#include <comdef.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include <mmdeviceapi.h>
#include <windows.h>

#include "OSAudio.h"
#include "Device.h"

using namespace std;

using byte = uint8_t;

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

// Error handling
#define HANDLE_ERROR(hres) \
            if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk) \
            if ((punk) != NULL) \
                { (punk)->Release(); (punk) = NULL; }

/**
 * A audio class that captures system wide audio on Windows
 */
class WindowsAudio : public OSAudio
{
    private:
        const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
        const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
        const IID IID_IAudioClient = __uuidof(IAudioClient);
        const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

        // System necessary variables
        HRESULT status;
        REFERENCE_TIME requestDuration = REFTIMES_PER_SEC;
        REFERENCE_TIME bufferDuration;

        IMMDeviceEnumerator* pEnumerator = NULL;
        IMMDeviceCollection* deviceCollection = NULL;

        // Audio data
        byte* pData;

    public:
        WindowsAudio();
        ~WindowsAudio();

        vector<Device*> getInputDevices();
        vector<Device*> getOutputDevices();

        static void test_capture(WindowsAudio* param);
        void capture();

        void setActiveOutputDevice(Device* device);
};

#endif