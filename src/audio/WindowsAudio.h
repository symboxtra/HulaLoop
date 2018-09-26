#ifndef WIN_AUDIO
#define WIN_AUDIO

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <Audioclient.h>
#include <tchar.h>
#include <strsafe.h>
#include <functiondiscoverykeys_devpkey.h>
#include <atlstr.h>
#include <strmif.h>
#include <dshow.h>
#include <DShow.h>

#include <atlbase.h>
#include <combaseapi.h>
#include <objbase.h>

#include <initguid.h>

#include <locale>
#include <codecvt>

#include <comdef.h>

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include <fstream>

#include <thread>

#include "OSAudio.h"
#include "Device.h"

using namespace std;

using byte = uint8_t; 

using f_int_t = int(*)(uint32_t, byte*);

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define HANDLE_ERROR(hres) \
            if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk) \
            if ((punk) != NULL) \
                { (punk)->Release(); (punk) = NULL; }

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

        byte* pData;



    public:

        WindowsAudio();
        ~WindowsAudio();

        vector<Device*> getInputDevices();
        vector<Device*> getOutputDevices();

        //static void test_capture(); //TODO: Figure out static context with comparison to Linux and thread creation
        static void test_capture(WindowsAudio* param, future<void> futureObj);
        void capture(future<void> futureObj);

        void setActiveOutputDevice(Device* device);
};

#endif