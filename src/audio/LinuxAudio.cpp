#include "LinuxAudio.h"

LinuxAudio::LinuxAudio()
{
    vector<Device *> t = getOutputDevices();
    if (t.empty())
    {
        cerr << "No devices found" << endl;
    }
    setActiveOutputDevice(t[0]);
}

vector<Device *> LinuxAudio::getInputDevices()
{
    clearDevices(this->iDevices);
    this->iDevices = getDevices(DeviceType::RECORD);
    return this->iDevices;
}

vector<Device *> LinuxAudio::getOutputDevices()
{
    clearDevices(this->oDevices);
    this->oDevices = getDevices(DeviceType::PLAYBACK);
    return this->oDevices;
}

void LinuxAudio::clearDevices(vector<Device *> devices)
{
    for(auto const & device : devices){
        // cout << device->getName() << endl;
        delete device;
    }
    devices.clear();
}

vector<Device *> LinuxAudio::getDevices(DeviceType type)
{
    // variables needed for the getting of devices to work
    vector<Device *> devices;
    snd_ctl_card_info_t *cardInfo;
    snd_pcm_info_t *subInfo;
    snd_ctl_t *handle;
    int subDevice;
    int cardNumber = -1;
    char cardName[64];
    char deviceID[64];

    // outer while gets all the sound cards
    while (snd_card_next(&cardNumber) >= 0 && cardNumber >= 0)
    {
        // open and init the sound card
        sprintf(cardName, "hw:%i", cardNumber);
        snd_ctl_open(&handle, cardName, 0);
        snd_ctl_card_info_alloca(&cardInfo);
        snd_ctl_card_info(handle, cardInfo);
        // inner while gets all the sound card subdevices
        subDevice = -1;
        while (snd_ctl_pcm_next_device(handle, &subDevice) >= 0 && subDevice >= 0)
        {
            // open and init the subdevices
            snd_pcm_info_alloca(&subInfo);
            snd_pcm_info_set_device(subInfo, subDevice);
            snd_pcm_info_set_subdevice(subInfo, 0);
            // check if the device is an input or output device
            if (type & DeviceType::RECORD)
            {
                snd_pcm_info_set_stream(subInfo, SND_PCM_STREAM_CAPTURE);
            }
            else
            {
                snd_pcm_info_set_stream(subInfo, SND_PCM_STREAM_PLAYBACK);
            }
            if (snd_ctl_pcm_info(handle, subInfo) >= 0)
            {
                sprintf(deviceID, "hw:%d,%d", cardNumber, subDevice);
                string deviceName = snd_ctl_card_info_get_name(cardInfo);
                string subDeviceName = snd_pcm_info_get_name(subInfo);
                string fullDeviceName = deviceName + ": " + subDeviceName;
                string * sDeviceID = new string(deviceID);
                devices.push_back(new Device(reinterpret_cast<uint32_t *>(sDeviceID), fullDeviceName, DeviceType::RECORD));
            }
        }
        snd_ctl_close(handle);
    }
    snd_config_update_free_global();
    return devices;
}

bool LinuxAudio::checkSamplingRate(Device *device)
{
    int err;                         // return for commands that might return an error
    snd_pcm_t *pcmHandle = NULL;     // default pcm handle
    snd_pcm_hw_params_t *param;

    char * id = (char*) reinterpret_cast<string*>(device->getID())->c_str();
    // TODO: FIX
    err = snd_pcm_open(&pcmHandle, id, SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0)
    {
        cerr << "Unable to open " << id << endl;
        return false;
    }
    // allocate hw params object and fill the pcm device with the default params
    snd_pcm_hw_params_alloca(&param);
    snd_pcm_hw_params_any(pcmHandle, param);

    //test the desired sample rate
    // TODO: insert actual sampling rate
    int testSamplingRate = 44100;
    if(snd_pcm_hw_params_test_rate(pcmHandle, param, testSamplingRate, 0) == 0){
        //the sampling rate is good
        cout << "SAMPLING RATE GOOD" << endl;
        return true;
    }
    return false;
}

void LinuxAudio::setActiveOutputDevice(Device *device)
{
    // Set the active output device
    this->activeOutputDevice = device;
    cout << checkSamplingRate(device) << endl;
    // Interrupt all threads and make sure they stop
    for (auto &t : execThreads)
    {
        // TODO: Find better way of safely terminating thread
        t.detach();
        t.~thread();
    }

    // Clean the threads after stopping all threads
    execThreads.clear();

    // Start up new threads with new selected device info

    // Start capture thread and add to thread vector
    execThreads.emplace_back(thread(&LinuxAudio::test_capture, this));

    // TODO: Add playback thread later

    // Detach new threads to run independently
    for (auto &t : execThreads)
    {
        t.detach();
    }
}

void LinuxAudio::test_capture(LinuxAudio *param)
{
    param->capture();
}

/*
   lengthOfRecording is in ms
   Device * recordingDevice is already formatted as hw:(int),(int)
   if Device is NULL then it chooses the default
   */
void LinuxAudio::capture()
{
    int err;                         // return for commands that might return an error
    snd_pcm_t *pcmHandle = NULL;     // default pcm handle
    string defaultDevice;            // default hw id for the device
    snd_pcm_hw_params_t *param;      // object to store our paramets (they are just the default ones for now)
    int audioBufferSize;             // size of the buffer for the audio
    byte *audioBuffer = NULL;        // buffer for the audio
    snd_pcm_uframes_t *temp = NULL;  // useless parameter because the api requires it

    // just writing to a buffer for now
    defaultDevice = "default";

    // open the pcm device
    err = snd_pcm_open(&pcmHandle, defaultDevice.c_str(), SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0)
    {
        cerr << "Unable to open " << defaultDevice << " exiting..." << endl;
        exit(1);
    }

    // allocate hw params object and fill the pcm device with the default params
    snd_pcm_hw_params_alloca(&param);
    snd_pcm_hw_params_any(pcmHandle, param);

    // set to interleaved mode, 16-bit little endian, 2 channels
    snd_pcm_hw_params_set_access(pcmHandle, param, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcmHandle, param, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(pcmHandle, param, 2);

    // getting the min sampling rate
    // TODO: REMOVE
    unsigned minVal;
    snd_pcm_hw_params_get_rate_min(param, &minVal, NULL);
    cout << "MINVAL: " << minVal  << endl;
     snd_pcm_hw_params_get_rate_max(param, &minVal, NULL);
    cout << "MAXVAL: " << minVal  << endl;

    // we set the sampling rate to whatever the user or device wants
    // TODO insert sample rate
    unsigned int sampleRate = 44100;
    snd_pcm_hw_params_set_rate_near(pcmHandle, param, &sampleRate, NULL);

    // set the period size to 32 TODO
    snd_pcm_uframes_t frame = FRAME_TIME;
    snd_pcm_hw_params_set_period_size_near(pcmHandle, param, &frame, NULL);

    // send the param to the the pcm device
    err = snd_pcm_hw_params(pcmHandle, param);
    if (err < 0)
    {
        cerr << "Unable to set parameters: " << defaultDevice << " exiting..." << endl;
        exit(1);
    }

    // get the size of one period
    snd_pcm_hw_params_get_period_size(param, &frame, NULL);

    // allocate memory for the buffer
    audioBufferSize = frame * 4;
    audioBuffer = (byte *)malloc(audioBufferSize);

    while (true)
    {
        break;
        while (callbackList.size() > 0)
        {
            // audioBuffer = (byte *)malloc(audioBufferSize);
            // read frames from the pcm
            err = snd_pcm_readi(pcmHandle, audioBuffer, frame);
            if (err == -EPIPE)
            {
                cerr << "Buffer overrun" << endl;
                snd_pcm_prepare(pcmHandle);
            }
            else if (err < 0)
            {
                cerr << "Read error" << endl;
            }
            else if (err != (int)frame)
            {
                cerr << "Read short, only read " << err << " bytes" << endl;
            }
            // write to standard output for now
            // TODO: change to not standard output
            for (int i = 0; i < callbackList.size(); i++)
            {
                thread(&ICallback::handleData, callbackList[i], audioBuffer, audioBufferSize).detach();
            }
            // free(audioBuffer);
            // audioBuffer = nullptr;
            /* err = write(1, audioBuffer, audioBufferSize);
            if(err != audioBufferSize)
            {
            cerr << "Write short, only wrote " << err << " bytes" << endl;
            } */
        }
    }
    // cleanup stuff
    snd_pcm_drain(pcmHandle);
    snd_pcm_close(pcmHandle);
    //free(audioBuffer);
}

LinuxAudio::~LinuxAudio()
{
    clearDevices(this->oDevices);
    clearDevices(this->iDevices);
    callbackList.clear();
    execThreads.clear();
}
