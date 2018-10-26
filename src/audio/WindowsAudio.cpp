#include "WindowsAudio.h"
#include "hlaudio/internal/HulaAudioError.h"

WindowsAudio::WindowsAudio()
{
}

/**
 * Receive the list of available input audio devices connected to the OS
 * and return them as Device instances
 *
 * @return vector of Device instances
 */
// TODO: Remove this after all branches are synced
vector<Device *> WindowsAudio::getInputDevices()
{
    return getDevices(DeviceType::RECORD);
}

/**
 * Receive the list of available output audio devices connected to the OS
 * and return them as Device instances
 *
 * @return vector of Device instances
 */
// TODO: Remove this after all branches are synced
vector<Device *> WindowsAudio::getOutputDevices()
{
    return getDevices(DeviceType::LOOPBACK);
}

vector<Device *> WindowsAudio::getDevices(DeviceType type)
{
    return {};
}

/**
 * Set the selected output device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
void WindowsAudio::setActiveOutputDevice(Device *device)
{
}

/**
 * Execution loop for loopback capture
 */
void WindowsAudio::capture()
{
    cout << "In Capture Mode" << endl; // TODO: Remove this later

    // Instantiate clients and services for audio capture
    IAudioCaptureClient *captureClient = NULL;
    IAudioClient *audioClient = NULL;
    WAVEFORMATEX *pwfx = NULL;
    IMMDevice *audioDevice = NULL;
    UINT32 numFramesAvailable;
    DWORD flags;
    char *buffer = (char *)malloc(500);
    uint32_t packetLength = 0;
    REFERENCE_TIME duration;
    REFERENCE_TIME req = REFTIMES_PER_SEC;

    // Setup capture environment
    status = CoInitialize(NULL);
    HANDLE_ERROR(status);

    // Creates a system instance of the device enumerator
    status = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void **)&pEnumerator);
    HANDLE_ERROR(status);

    // Select the current active output device
    status = pEnumerator->GetDevice(reinterpret_cast<LPCWSTR>(activeOutputDevice->getID()), &audioDevice);
    // status = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audioDevice);
    HANDLE_ERROR(status);
    cout << "Selected Device: " << activeOutputDevice->getName() << endl; // TODO: Remove this later

    // Activate the IMMDevice
    status = audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void **)&audioClient);
    HANDLE_ERROR(status);

    // Not sure what this does yet!?
    status = audioClient->GetMixFormat(&pwfx);
    HANDLE_ERROR(status);

    // Initialize the audio client in loopback mode and set audio engine format
    status = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, req, 0, pwfx, NULL);
    HANDLE_ERROR(status);

    // Gets the maximum capacity of the endpoint buffer (audio frames)
    status = audioClient->GetBufferSize(&captureBufferSize);
    HANDLE_ERROR(status)

    // Gets the capture client service under the audio client instance
    status = audioClient->GetService(IID_IAudioCaptureClient, (void **)&captureClient);
    HANDLE_ERROR(status);

    // Start the audio stream
    status = audioClient->Start();
    HANDLE_ERROR(status);

    // Sleep duration
    duration = (double)REFTIMES_PER_SEC * captureBufferSize / pwfx->nSamplesPerSec;

    // Continue loop under process ends
    while (!this->endCapture)
    {
        Sleep(duration / (REFTIMES_PER_MILLISEC * 2));

        // Get the packet size of the next captured buffer
        status = captureClient->GetNextPacketSize(&packetLength);
        HANDLE_ERROR(status);

        while (packetLength != 0)
        {
            // Get the captured buffer
            status = captureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
            HANDLE_ERROR(status);

            // Detect silent noise
            if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
            {
                pData = NULL;
            }

            // Copy to ringbuffers
            this->copyToBuffers(pData, numFramesAvailable * NUM_CHANNELS * sizeof(SAMPLE));

            // Release buffer after data is captured and handled
            status = captureClient->ReleaseBuffer(numFramesAvailable);
            HANDLE_ERROR(status);

            // Get next packet size of captured buffer
            status = captureClient->GetNextPacketSize(&packetLength);
            HANDLE_ERROR(status);
        }
    }

    // Stop the client capture once process exits
    status = audioClient->Stop();
    HANDLE_ERROR(status);

    // goto label for exiting loop in-case of error
Exit:
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pEnumerator);
    SAFE_RELEASE(audioDevice);
    SAFE_RELEASE(audioClient);
    SAFE_RELEASE(captureClient);

    _com_error err(status);
    LPCTSTR errMsg = err.ErrorMessage();
    cerr << "\nError: " << errMsg << endl;
    exit(1);

    // TODO: Handle error accordingly
}

/**
 * TODO: Fill in with something
 */
bool WindowsAudio::checkRates(Device *device)
{
    return true;
}

/**
 * Clear all global pointers
 */
WindowsAudio::~WindowsAudio()
{
    printf("%sWindowsAudio destructor called\n", HL_PRINT_PREFIX);

    delete pEnumerator;
    delete deviceCollection;

    delete pData;
}