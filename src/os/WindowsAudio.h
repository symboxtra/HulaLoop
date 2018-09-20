#ifndef WIN_AUDIO
#define WIN_AUDIO

#include "Device.h"

#include <string>
#include <vector>
#include <windows.h>
#include <tchar.h>

using byte = uint8_t; 

using f_int_t = int(*)(int);

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres) \
            if (FAILED(hrs)) { goto Exit; }
#define SAFE_RELEASE(punk) \
            if ((punk) != NULL) \
                { (punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

class WindowsAudio {

private:

    //TODO: Figure out device class

    Device* activeOutputDevice;
    Device* activeInputDevice;
    uint32_t captureBufferSize;
    vector<f_int_t> callbackList;

    vector<Device*> deviceList;

    // System necessary variables
    HRESULT status;
    REFERENCE_TIME requestDuration = REFTIMES_PER_SEC;
    REFERENCE_TIME bufferDuration;

    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDeviceCollecton *deviceCollection = NULL;
    IAudioClient *pAudioClient = NULL;
    IAudioCaptureClient *pCaptureClient = NULL;
    WAVEFORMATEX *pwfx = NULL;



public:

    WindowsAudio();
    ~WindowsAudio();

    vector<Device*> getAudioDevices();
    vector<Device*> getInputDevices();
    vector<Device*> getOutputDevices();

    void setBufferSize(uint32_t size);

    void setActiveRecordDevice(Device* device);
    void setActiveOutputDevice(Device* device);

    void addBufferReadyCallback(f_int_t callFunction);
};

#endif