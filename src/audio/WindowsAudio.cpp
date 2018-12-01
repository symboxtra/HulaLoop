#include "WindowsAudio.h"
#include "hlaudio/internal/HulaAudioError.h"
#include "hlaudio/internal/HulaAudioSettings.h"

using namespace hula;

WindowsAudio::WindowsAudio()
{
    // Initialize PortAudio
    int err = Pa_Initialize();
    if (err != paNoError)
    {
        hlDebugf("PortAudio failed to initialize.\n");
        hlDebugf("PortAudio: %s\n", Pa_GetErrorText(err));
        exit(1); // TODO: Handle error
    }

    pa_status = paNoError;
}

/**
 * Receive the list of available record, playback and/or loopback audio devices
 * connected to the OS and return them as Device instances
 *
 * @param type DeviceType that is combination from the DeviceType enum
 * @return std::vector<Device*> A list of Device instances that carry the necessary device information
 */
std::vector<Device *> WindowsAudio::getDevices(DeviceType type)
{
    // Check if the following enums are set in the params
    bool isLoopSet = (type & DeviceType::LOOPBACK) == DeviceType::LOOPBACK;
    bool isRecSet = (type & DeviceType::RECORD) == DeviceType::RECORD;
    bool isPlaySet = (type & DeviceType::PLAYBACK) == DeviceType::PLAYBACK;

    // Vector to store acquired list of output devices
    std::vector<Device *> deviceList;

    // Get devices from WASAPI if loopback and/or playback devices
    // are requested
    if (isLoopSet)
    {
        // Setup capture environment
        status = CoInitialize(nullptr);
        HANDLE_ERROR(status);

        // Creates a system instance of the device enumerator
        status = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void **)&pEnumerator);
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
            DWORD state = 0;
            LPWSTR id = nullptr;
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
            std::wstring fun(varName.pwszVal);
            std::string str(fun.begin(), fun.end());

            // Create instance of Device using acquired data
            DeviceID deviceId;
            deviceId.windowsID = id;
            Device *audio = new Device(deviceId, str, (DeviceType)(DeviceType::LOOPBACK | DeviceType::PLAYBACK));

            // Add to devicelist
            deviceList.push_back(audio);

            SAFE_RELEASE(device);
        }
    }

    HulaAudioSettings *s = HulaAudioSettings::getInstance();

    // Get devices from PortAudio if record or playback devices are requested
    if (isPlaySet || isRecSet)
    {

        // Initialize PortAudio and update audio devices
        pa_status = Pa_Initialize();
        HANDLE_PA_ERROR(pa_status);

        // Get the total count of audio devices
        int numDevices = Pa_GetDeviceCount();
        if (numDevices < 0)
        {
            pa_status = numDevices;
            HANDLE_PA_ERROR(pa_status);
        }

        for (uint32_t i = 0; i < numDevices; i++)
        {
            const PaDeviceInfo *paDevice = Pa_GetDeviceInfo(i);
            DeviceType checkType = (DeviceType) 0;

            if (isPlaySet && paDevice->maxOutputChannels > 0 && paDevice->hostApi == Pa_GetDefaultHostApi())
            {
                checkType = (DeviceType)(checkType | DeviceType::PLAYBACK);
            }
            if (isRecSet && paDevice->maxInputChannels > 0 && paDevice->hostApi == Pa_GetDefaultHostApi())
            {
                checkType = (DeviceType)(checkType | DeviceType::RECORD);
            }

            // Create HulaLoop style device and add to vector
            // This needs to be freed elsewhere
            if (checkType)
            {
                if (checkType == DeviceType::RECORD && !s->getShowRecordDevices())
                {
                    continue;
                }

                DeviceID id;
                id.portAudioID = i;
                Device *hlDevice = new Device(id, std::string(paDevice->name), checkType);
                deviceList.push_back(hlDevice);
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
        hlDebug() << "WASAPI_Error: " << errMsg << std::endl;
        return {};
    }
    else if (pa_status != paNoError)
    {
        hlDebug() << "PORTAUDIO_Error: " << Pa_GetErrorText(pa_status) << std::endl;
        return {};
    }
    else
    {
        return deviceList;
    }
}

/**
 * This routine will be called by the PortAudio engine when audio is needed.
 * It may be called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
 */
static int paRecordCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{
    WindowsAudio *obj = (WindowsAudio *)userData;
    float *samples = (float *)inputBuffer;

    // Prevent unused variable warnings.
    (void)outputBuffer;
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    // TODO: Make sure this calculation is right
    obj->copyToBuffers(samples, framesPerBuffer * NUM_CHANNELS);

    return paContinue;
}

/**
 * This routine will be called by the PortAudio engine when audio is needed.
 * It may be called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
 */
static int paPlayCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{
    WindowsAudio *obj = (WindowsAudio *)userData;

    // Prevent unused variable warnings.
    //(void)outputBuffer;
    //(void)timeInfo;
    //(void)statusFlags;
    //(void)userData;
    SAMPLE* wptr = (SAMPLE*)outputBuffer;

    // TODO: Make sure this calculation is right
    void *ptr[2] = {0};
    ring_buffer_size_t sizes[2] = {0};
    ring_buffer_size_t samplesRead = obj->playbackBuffer->directRead((ring_buffer_size_t)(framesPerBuffer * NUM_CHANNELS), ptr + 0, sizes + 0, ptr + 1, sizes + 1);
    if (sizes[1] > 0)
    {
        memcpy(wptr, (float *)ptr[0], sizes[0] * sizeof(float));
        wptr = wptr + (sizes[0] * sizeof(float));
        memcpy(wptr, (float *)ptr[1], sizes[1] * sizeof(float));
    }
    else
    {
        memcpy(wptr, (float *)ptr[0], sizes[0] * sizeof(float));
    }

    return paContinue;
}

/**
 * Execution loop for loopback capture
 */
void WindowsAudio::capture()
{
    // Check if the following enums are set in the params
    bool isLoopSet = (activeInputDevice->getType() & DeviceType::LOOPBACK) == DeviceType::LOOPBACK;
    bool isRecSet = (activeInputDevice->getType() & DeviceType::RECORD) == DeviceType::RECORD;
    bool isPlaySet = (activeInputDevice->getType() & DeviceType::PLAYBACK) == DeviceType::PLAYBACK;

    hlDebug() << "In Capture Mode" << std::endl;

    if (isLoopSet)
    {
        // Instantiate clients and services for audio capture
        IAudioCaptureClient *captureClient = nullptr;
        IAudioClient *audioClient = nullptr;
        WAVEFORMATEX *pwfx = nullptr;
        IMMDevice *audioDevice = nullptr;
        UINT32 numFramesAvailable;
        DWORD flags;
        char *buffer = (char *)malloc(500);
        uint32_t packetLength = 0;
        DWORD duration;
        REFERENCE_TIME req = REFTIMES_PER_SEC;

        // Setup capture environment
        status = CoInitialize(nullptr);
        HANDLE_ERROR(status);

        // Creates a system instance of the device enumerator
        status = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void **)&pEnumerator);
        HANDLE_ERROR(status);

        // Select the current active record/loopback device
        status = pEnumerator->GetDevice(activeInputDevice->getID().windowsID, &audioDevice);
        // status = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audioDevice);
        HANDLE_ERROR(status);
        hlDebug() << "Selected Device: " << activeInputDevice->getName() << std::endl;

        // Activate the IMMDevice
        status = audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void **)&audioClient);
        HANDLE_ERROR(status);

        // Not sure what this does yet!?
        status = audioClient->GetMixFormat(&pwfx);
        HANDLE_ERROR(status);

        // Initialize the audio client in loopback mode and set audio engine format
        status = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, req, 0, pwfx, nullptr);
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
        duration = (DWORD)REFTIMES_PER_SEC * captureBufferSize / pwfx->nSamplesPerSec;

        // Continue loop under process ends
        // Each loop fills half of the shared buffer
        while (!this->endCapture.load())
        {
            // Sleep for half the buffer duration
            Sleep(duration / (REFTIMES_PER_MILLISEC * 2));

            // Get the packet size of the next captured buffer
            status = captureClient->GetNextPacketSize(&packetLength);
            HANDLE_ERROR(status);

            while (packetLength != 0)
            {
                // Get the captured buffer
                status = captureClient->GetBuffer(&pData, &numFramesAvailable, &flags, nullptr, nullptr);
                HANDLE_ERROR(status);

                this->copyToBuffers((float *)pData, numFramesAvailable * NUM_CHANNELS);

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

        if (FAILED(status))
        {
            _com_error err(status);
            LPCTSTR errMsg = err.ErrorMessage();
            hlDebug() << "\nError: " << errMsg << std::endl;
            exit(1);
            // TODO: Handle error accordingly
        }
    }
    else if (isRecSet)
    {
        PaStreamParameters inputParameters = {0};
        PaStream *stream;
        PaError err = paNoError;

        inputParameters.device = this->activeInputDevice->getID().portAudioID;
        if (inputParameters.device == paNoDevice)
        {
            hlDebugf("No device found.\n");
            exit(1); // TODO: Handle error
        }

        // Setup the stream for the selected device
        inputParameters.channelCount = Pa_GetDeviceInfo(inputParameters.device)->maxInputChannels;
        inputParameters.sampleFormat = PA_SAMPLE_TYPE;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = nullptr;

        err = Pa_OpenStream(
                  &stream,
                  &inputParameters,
                  nullptr, // &outputParameters
                  SAMPLE_RATE,
                  FRAMES_PER_BUFFER,
                  paClipOff, // We won't output out of range samples so don't bother clipping them
                  paRecordCallback,
                  this // Pass our instance in
              );

        if (err != paNoError)
        {
            hlDebugf("Could not open Port Audio device stream.\n");
            hlDebugf("PortAudio: %s\n", Pa_GetErrorText(err));
            exit(1); // TODO: Handle error
        }

        // Start the stream
        err = Pa_StartStream(stream);
        if (err != paNoError)
        {
            hlDebugf("Could not start Port Audio device stream.\n");
            hlDebugf("PortAudio: %s\n", Pa_GetErrorText(err));
            exit(1); // TODO: Handle error
        }

        hlDebugf("Capture keep-alive\n");

        while (!this->endCapture.load())
        {
            // Keep this thread alive
            // The second half of this function could be moved to a separate
            // function like endCapture() so that we don't have to keep this thread alive.
            Pa_Sleep(10);
        }

        hlDebugf("Capture thread ended keep-alive.\n\n");

        if (err != paNoError)
        {
            hlDebugf("Error during read from device stream.\n");
            hlDebugf("PortAudio: %s\n", Pa_GetErrorText(err));
            exit(1); // TODO: Handle error
        }

        err = Pa_CloseStream(stream);
        if (err != paNoError)
        {
            hlDebugf("Could not close Port Audio device stream.\n");
            hlDebugf("PortAudio: %s\n", Pa_GetErrorText(err));
            exit(1); // TODO: Handle error
        }
    }
}

/**
 * Checks the sampling rate and bit depth of the device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
bool WindowsAudio::checkDeviceParams(Device *activeDevice)
{
    return true; // TODO: Remove this and add PortAudio checks and change deviceID code

    HRESULT status;
    IMMDevice *device = nullptr;
    PROPVARIANT prop;
    IPropertyStore *store = nullptr;
    PWAVEFORMATEX deviceProperties;
    // Setup capture environment
    status = CoInitialize(nullptr);
    HANDLE_ERROR(status);
    // Creates a system instance of the device enumerator
    status = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void **)&pEnumerator);
    HANDLE_ERROR(status);
    // Select the current active record/loopback device
    status = pEnumerator->GetDevice(activeDevice->getID().windowsID, &device);
    HANDLE_ERROR(status);
    status = device->OpenPropertyStore(STGM_READ, &store);
    HANDLE_ERROR(status);
    status = store->GetValue(PKEY_AudioEngine_DeviceFormat, &prop);
    HANDLE_ERROR(status);
    deviceProperties = (PWAVEFORMATEX)prop.blob.pBlobData;
    // Check number of channels
    if (deviceProperties->nChannels != HulaAudioSettings::getInstance()->getNumberOfChannels())
    {
        hlDebug() << "Invalid number of channels" << std::endl;
        return false;
    }
    // Check sample rate
    if (deviceProperties->nSamplesPerSec != HulaAudioSettings::getInstance()->getSampleRate())
    {
        hlDebug() << "Invalid sample rate" << std::endl;
        return false;
    }

    return true;

Exit:
    hlDebug() << "WASAPI Init Error" << std::endl;
    return false;
}

/**
 * Clear all global pointers
 */
WindowsAudio::~WindowsAudio()
{
    hlDebugf("WindowsAudio destructor called\n");

    // Close the Port Audio session
    PaError err = Pa_Terminate();
    if (err != paNoError)
    {
        hlDebugf("Could not terminate Port Audio session.\n");
        hlDebugf("PortAudio: %s\n", Pa_GetErrorText(err));
    }
}
