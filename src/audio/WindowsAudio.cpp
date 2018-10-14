#include "WindowsAudio.h"

WindowsAudio::WindowsAudio()
{
    // TODO: Remove this once triggered from upper layers
    vector<Device*> t = getOutputDevices();

    if(!t.empty())
        setActiveOutputDevice(t[0]);
    else
        cerr << "No Devices Found!" << endl;
}

/**
 * Receive the list of available input audio devices connected to the OS
 * and return them as Device instances
 *
 * @return vector of Device instances
 */
vector<Device*> WindowsAudio::getInputDevices()
{
    // Vector to store acquired list of input audio devices
    vector<Device*> deviceList;

    // Make a system call to the OS to receive device list
    uint16_t deviceCount = waveInGetNumDevs();
    if(deviceCount > 0)
    {
        for(int i = 0;i < deviceCount;i++)
        {
            // Get each device, create Device object and add to the device list
            WAVEINCAPSW waveInCaps;
            waveInGetDevCapsW( i, &waveInCaps, sizeof( WAVEINCAPSW ) );
            wstring temp(waveInCaps.szPname);
            string str(temp.begin(), temp.end());

            cout << str << endl;
            deviceList.push_back(new Device(NULL, str, DeviceType::RECORD));
        }
    }
    else
    {
        cerr << "Input Device Error!" << endl;
    }

    return deviceList;
}

/**
 * Receive the list of available output audio devices connected to the OS
 * and return them as Device instances
 *
 * @return vector of Device instances
 */
vector<Device*> WindowsAudio::getOutputDevices()
{
    // Vector to store acquired list of output devices
    vector<Device*> deviceList;

    // Setup capture environment
    status = CoInitialize(NULL);
    HANDLE_ERROR(status);

    // Creates a system instance of the device enumerator
    status = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
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
    for(UINT i = 0;i < count;i++)
    {
        // Initialize variables for current loop
        IMMDevice* device;
        DWORD state = NULL;
        LPWSTR id = NULL;
        IPropertyStore* propKey;
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
        Device* audio = new Device(reinterpret_cast<uint32_t*>(id), str, (DeviceType)(DeviceType::LOOPBACK | DeviceType::PLAYBACK));

        // Add to devicelist
        deviceList.push_back(audio);

        SAFE_RELEASE(device);
    }

// goto label for exiting loop in-case of error
Exit:
    SAFE_RELEASE(pEnumerator)

    // Output error to stdout
    // TODO: Handle error accordingly
    if(FAILED(status))
    {
        _com_error err(status);
        LPCTSTR errMsg = err.ErrorMessage();
        cerr << "Error: " << errMsg << endl;
        return {};
    }
    else
        return deviceList;
}

/**
 * Set the selected output device and restart capture threads with
 * new device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
void WindowsAudio::setActiveOutputDevice(Device* device)
{
    // Set the active output device
    this->activeOutputDevice = device;

    // Interrupt all threads and make sure they stop
    for(auto& t : execThreads)
    {
        // TODO: Find better way of safely terminating thread
        t.detach();
        t.~thread();
    }

    // Clean the threads after stopping all threads
    execThreads.clear();

    // Start up new threads with new selected device info

    // Start capture thread and add to thread vector
    execThreads.emplace_back(thread(&WindowsAudio::test_capture, this));

    // TODO: Add playback thread later

    // Detach new threads to run independently
    for(auto& t : execThreads)
        t.detach();
}

/**
 * Static function in the current instance of the class
 * to allow thread execution
 *
 * @param _this Instance of the current object
 */
void WindowsAudio::test_capture(WindowsAudio* _this)
{
    _this->capture();
}

/**
 * Execution loop for loopback capture
 */
void WindowsAudio::capture()
{
    cout << "In Capture Mode" << endl;

    // Instantiate clients and services for audio capture
    IAudioCaptureClient* captureClient = NULL;
    IAudioClient* audioClient = NULL;
    WAVEFORMATEX* pwfx = NULL;
    IMMDevice* audioDevice = NULL;
    UINT32 numFramesAvailable;
    DWORD flags;
    char* buffer = (char*)malloc(500);
    uint32_t packetLength = 0;
    REFERENCE_TIME duration;
    REFERENCE_TIME req = REFTIMES_PER_SEC;

    // Setup capture environment
    status = CoInitialize(NULL);
    HANDLE_ERROR(status);

    // Creates a system instance of the device enumerator
    status = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
    HANDLE_ERROR(status);

    // Select the current active output device
    status = pEnumerator->GetDevice(reinterpret_cast<LPCWSTR>(activeOutputDevice->getID()), &audioDevice);
    // status = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audioDevice);
    HANDLE_ERROR(status);
    cout << "Selected Device: " << activeOutputDevice->getName() << endl;

    // Activate the IMMDevice
    status = audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&audioClient);
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
    status = audioClient->GetService(IID_IAudioCaptureClient, (void**)&captureClient);
    HANDLE_ERROR(status);

    // Start the audio stream
    status = audioClient->Start();
    HANDLE_ERROR(status);

    // Sleep duration
    duration = (double)REFTIMES_PER_SEC * captureBufferSize / pwfx->nSamplesPerSec;

    // Continue loop under process ends
    while(true)
    {
        // Only capture data if anyone has listening callbacks
        while(callbackList.size() > 0)
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
                if(flags & AUDCLNT_BUFFERFLAGS_SILENT)
                    pData = NULL;

                // Execute callbacks
                for(int i = 0;i < callbackList.size();i++)
                {
                    // TODO: Check if making it asynchronous will cause data overlap problems
                    thread(&ICallback::handleData, callbackList[i], pData, numFramesAvailable).detach();
                }

                // Release buffer after data is captured and handled
                status = captureClient->ReleaseBuffer(numFramesAvailable);
                HANDLE_ERROR(status);

                // Get next packet size of captured buffer
                status = captureClient->GetNextPacketSize(&packetLength);
                HANDLE_ERROR(status);
            }
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
 * Clear all global pointers
 */
WindowsAudio::~WindowsAudio()
{
    delete activeInputDevice;
    delete activeOutputDevice;

    callbackList.clear();
    execThreads.clear();

    delete pEnumerator;
    delete deviceCollection;

    delete pData;
}