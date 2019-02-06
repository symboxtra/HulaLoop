#ifndef HL_WIN_AUDIO_H
#define HL_WIN_AUDIO_H

// Windows Audio
#include <Audioclient.h>
#include <comdef.h>
#include <endpointvolume.h>
#include <initguid.h>
#include <mmdeviceapi.h>
#include <windows.h>

// Do not move this before mmdeviceapi.h
#include <functiondiscoverykeys_devpkey.h>

// PortAudio
#include <portaudio.h>

// System
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

#include "hlaudio/internal/Device.h"
#include "hlaudio/internal/OSAudio.h"

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

// Error handling
#define HANDLE_ERROR(hres) \
            if (FAILED(hres)) { goto Exit; }
#define HANDLE_PA_ERROR(hres) \
            if (hres != paNoError) { goto Exit; }
#define SAFE_RELEASE(punk) \
            if ((punk) != nullptr) \
                { (punk)->Release(); (punk) = nullptr; }

namespace hula
{
    /**
     * A audio class that captures system wide audio on Windows
     */
    class WindowsAudio : public OSAudio {

        private:
            const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
            const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
            const IID IID_IAudioClient = __uuidof(IAudioClient);
            const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);
            const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

            // System necessary variables
            HRESULT status;
            PaError pa_status;

            REFERENCE_TIME requestDuration = REFTIMES_PER_SEC;
            REFERENCE_TIME bufferDuration;

            IMMDeviceEnumerator *pEnumerator = nullptr;
            IMMDeviceCollection *deviceCollection = nullptr;

            // Audio data
            uint8_t *pData;
            uint8_t *rData;

        public:
            WindowsAudio();
            ~WindowsAudio();

            bool checkDeviceParams(Device *device);
            std::vector<Device *> getDevices(DeviceType type);

            void capture();
    };
}

#endif // END HL_WIN_AUDIO_H