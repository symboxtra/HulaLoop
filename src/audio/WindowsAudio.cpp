#include "WindowsAudio.h"
#include "hlaudio/internal/HulaAudioError.h"

WindowsAudio::WindowsAudio()
{
    pa_status = paNoError;
    activeInputDevice = NULL;
}

/**
 * Receive the list of available record, playback and/or loopback audio devices
 * connected to the OS and return them as Device instances
 *
 * @param a type or a combination of type of devices to be received (LOOPBACK, PLAYBACK, RECORD)
 * @return vector of Device instances
 */
vector<Device*> WindowsAudio::getDevices(DeviceType type)
{

    // Check if the following enums are set in the params
    bool isLoopSet = (type & DeviceType::LOOPBACK) == DeviceType::LOOPBACK;
    bool isRecSet = (type & DeviceType::RECORD) == DeviceType::RECORD;
    bool isPlaySet = (type & DeviceType::PLAYBACK) == DeviceType::PLAYBACK;

    // Vector to store acquired list of output devices
    vector<Device*> deviceList;

    // Get devices from WASAPI if loopback and/or playback devices
    // are requested
    if(isLoopSet | isPlaySet)
    {
        // Setup capture environment
        status = CoInitialize(NULL);
        HANDLE_ERROR(status);

        // Creates a system instance of the device enumerator
        status = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void **)&pEnumerator);
        HANDLE_ERROR(status);

        // Get the collection of all devices
        status = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &deviceCollection);
        HANDLE_ERROR(status);

        // Get the size of the collection
        UINT count = 0;
        status = deviceCollection->GetCount(&count);
        HANDLE_ERROR(status);

        // Iterate through the collection and store the necessary information into
        // the device vector
        for (UINT i = 0; i < count; i++)
        {
            // Initialize variables for current loop
            IMMDevice *device;
            DWORD state = NULL;
            LPWSTR id = NULL;
            IPropertyStore *propKey;
            PROPVARIANT varName;
            PropVariantInit(&varName);

            // Get the IMMDevice at the specified index
            status = deviceCollection->Item(i, &device);
            HANDLE_ERROR(status);

            // Get the state of the IMMDevice
            status = device->GetState(&state);
            HANDLE_ERROR(status);

            // Get the ID of the IMMDevice
            status = device->GetId(&id);
            HANDLE_ERROR(status);

            // Get device name using PropertyStore
            status = device->OpenPropertyStore(STGM_READ, &propKey);
            HANDLE_ERROR(status);
            status = propKey->GetValue(PKEY_Device_FriendlyName, &varName);
            HANDLE_ERROR(status);

            // Convert wide-char string to std::string
            wstring fun(varName.pwszVal);
            string str(fun.begin(), fun.end());

            // Create instance of Device using acquired data
            Device *audio = new Device(reinterpret_cast<uint32_t *>(id), str, (DeviceType)(DeviceType::LOOPBACK | DeviceType::PLAYBACK));

            // Add to devicelist
            deviceList.push_back(audio);

            SAFE_RELEASE(device);
        }
    }

    // Get devices from PortAudio if record devices are requested
    if(isRecSet)
    {

        // Initialize PortAudio and update audio devices
        pa_status = Pa_Initialize();
        HANDLE_PA_ERROR(pa_status);

        // Get the total count of audio devices
        int numDevices = Pa_GetDeviceCount();
        if(numDevices < 0)
        {
            pa_status = numDevices;
            HANDLE_PA_ERROR(pa_status);
        }

        //
        const PaDeviceInfo *deviceInfo;
        for(int i = 0;i < numDevices;i++)
        {
            deviceInfo = Pa_GetDeviceInfo(i);

            if(deviceInfo->maxInputChannels != 0 && deviceInfo->hostApi == (Pa_GetDefaultHostApi()+1))
            {
                // Create instance of Device using acquired data
                Device* audio = new Device(NULL, string(deviceInfo->name), DeviceType::RECORD);

                // Add to devicelist
                deviceList.push_back(audio);

                // Print some debug device info for now
            // TODO: Remove
            cout << "Device #" << i + 1 << ": " << deviceInfo->name << endl;
            cout << "Input Channels: " << deviceInfo->maxInputChannels << endl;
            cout << "Output Channels: " << deviceInfo->maxOutputChannels << endl;
            cout << "Default Sample Rate: " << deviceInfo->defaultSampleRate << endl;
            cout << endl;
            }
        }

        pa_status = Pa_Terminate();
        HANDLE_PA_ERROR(pa_status);
    }

Exit:
    SAFE_RELEASE(pEnumerator)

    // Output error to stdout
    // TODO: Handle error accordingly
    if (FAILED(status))
    {
        _com_error err(status);
        LPCTSTR errMsg = err.ErrorMessage();
        cerr << "WASAPI_Error: " << errMsg << endl;
        return {};
    }
    else if (pa_status != paNoError)
    {
        cerr << "PORTAUDIO_Error: " << Pa_GetErrorText(pa_status) << endl;
        return {};
    }
    else
    {
        return deviceList;
    }
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

    // Select the current active record/loopback device
    status = pEnumerator->GetDevice(reinterpret_cast<LPCWSTR>(activeInputDevice->getID()), &audioDevice);
    // status = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audioDevice);
    HANDLE_ERROR(status);
    cout << "Selected Device: " << activeInputDevice->getName() << endl; // TODO: Remove this later

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

            // Copy to ringbuffers
            this->copyToBuffers((void*)pData, numFramesAvailable * NUM_CHANNELS * sizeof(SAMPLE));

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

    if(FAILED(status))
    {
        _com_error err(status);
        LPCTSTR errMsg = err.ErrorMessage();
        cerr << "\nError: " << errMsg << endl;
        exit(1);
        // TODO: Handle error accordingly
    }
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
}