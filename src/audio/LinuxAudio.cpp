#include "LinuxAudio.h"

LinuxAudio::LinuxAudio()
{
    vector<Device *> t = getOutputDevices();
    if (t.empty())
    {
        cerr << "No devices found" << endl;
    }
    // setActiveOutputDevice(t[0]);
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
                string *sDeviceID = new string(deviceID);
                devices.push_back(new Device(reinterpret_cast<uint32_t *>(sDeviceID), fullDeviceName, DeviceType::RECORD));
            }
        }
        snd_ctl_close(handle);
    }
    snd_config_update_free_global();
    return devices;
}

bool LinuxAudio::checkRates(Device *device)
{
    int err;                         // return for commands that might return an error
    snd_pcm_t *pcmHandle = NULL;     // default pcm handle
    snd_pcm_hw_params_t *param;      // defaults param for the pcm
    snd_pcm_format_t format;         // format that user chooses
    unsigned samplingRate;            // sampling rate the user choooses
    bool samplingRateValid;          // bool that gets set if the sampling rate is valid
    bool formatValid;                // bool that gets set if the format is valid

    // device id
    char *id = (char *) reinterpret_cast<string *>(device->getID())->c_str();
    cout << id << endl;
    // open pcm device
    err = snd_pcm_open(&pcmHandle, id, SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0)
    {
        cerr << "Unable to test device: " << id << endl;
        return false;
    }

    // allocate hw params object and fill the pcm device with the default params
    snd_pcm_hw_params_alloca(&param);
    snd_pcm_hw_params_any(pcmHandle, param);

    // test the desired sample rate
    // TODO: insert actual sampling rate
    samplingRate = 44100;
    samplingRateValid = snd_pcm_hw_params_test_rate(pcmHandle, param, samplingRate, 0) == 0;

    // test the desired format (bit depth)
    format = SND_PCM_FORMAT_S16_LE;
    formatValid = snd_pcm_hw_params_test_format(pcmHandle, param, format) == 0;

    // clean up
    snd_pcm_drain(pcmHandle);
    snd_pcm_close(pcmHandle);
    snd_config_update_free_global();
    if (samplingRateValid && formatValid)
    {
        cout << "Sampling rate and format valid" << endl;
        return true;
    }
    cout << "Something invalid" << endl;
    return false;
}

void LinuxAudio::setActiveOutputDevice(Device *device)
{
    // Set the active output device
    this->activeOutputDevice = device;
    cout << checkRates(device) << endl;
    // Interrupt all threads and make sure they stop
    // for (auto &t : execThreads)
    // {
    //     // TODO: Find better way of safely terminating thread
    //     t.detach();
    //     t.~thread();
    // }

    // // Clean the threads after stopping all threads
    // execThreads.clear();

    // // Start up new threads with new selected device info

    // // Start capture thread and add to thread vector
    // execThreads.emplace_back(thread(&LinuxAudio::test_capture, this));

    // // TODO: Add playback thread later

    // // Detach new threads to run independently
    // for (auto &t : execThreads)
    // {
    //     t.detach();
    // }
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
    int framesRead = 0;              // amount of frames read

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
    snd_pcm_hw_params_set_format(pcmHandle, param, SND_PCM_FORMAT_FLOAT_LE);
    snd_pcm_hw_params_set_channels(pcmHandle, param, 2);

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
    audioBufferSize = frame * NUM_CHANNELS * sizeof(SAMPLE);
    audioBuffer = (byte *)malloc(audioBufferSize);

    while (true)
    {
        // while (callbackList.size() > 0)
        // {
        // read frames from the pcm
        framesRead = snd_pcm_readi(pcmHandle, audioBuffer, frame);
        if (framesRead == -EPIPE)
        {
            cerr << "Buffer overrun" << endl;
            snd_pcm_prepare(pcmHandle);
        }
        else if (framesRead < 0)
        {
            cerr << "Read error" << endl;
        }
        else if (framesRead != (int)frame)
        {
            cerr << "Read short, only read " << framesRead << " bytes" << endl;
        }
        copyToBuffers(audioBuffer,  framesRead * NUM_CHANNELS * sizeof(SAMPLE));
    }
    // cleanup stuff
    snd_pcm_drain(pcmHandle);
    snd_pcm_close(pcmHandle);
    free(audioBuffer);
}

LinuxAudio::~LinuxAudio()
{
    // callbackList.clear();
    // execThreads.clear();
}
