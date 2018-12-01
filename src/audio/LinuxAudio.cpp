#include "LinuxAudio.h"
#include "hlaudio/internal/HulaAudioError.h"
#include "hlaudio/internal/HulaAudioSettings.h"

using namespace hula;

/**
 * Construct a new instance of LinuxAudio.
 */
LinuxAudio::LinuxAudio()
{
}

/**
 * Fetch devices of the specified type.
 * These devices must be deleted by the caller using the
 * Device::deleteDevices() method.
 *
 * The list is designed for one-time use. Get the list,
 * use the device, delete the list immediately.
 *
 * DO NOT STORE THIS as it may become out-of-date.
 *
 * @return List of Device objects
 */
std::vector<Device *> LinuxAudio::getDevices(DeviceType type)
{
    // variables needed for the getting of devices to work
    std::vector<Device *> devices;
    snd_ctl_card_info_t *cardInfo;
    snd_pcm_info_t *subInfo;
    snd_ctl_t *handle;
    int subDevice;
    int cardNumber = -1;
    std::string cardName;

    // check what devices we need to get
    bool loopSet = (type & DeviceType::LOOPBACK) == DeviceType::LOOPBACK;
    bool recSet = (type & DeviceType::RECORD) == DeviceType::RECORD;
    bool playSet = (type & DeviceType::PLAYBACK) == DeviceType::PLAYBACK;

    // add pavucontrol to loopback for now
    if (loopSet)
    {
        DeviceID id;
        id.linuxID = std::string("default");
        std::string temp("Pulse Audio Volume Control");
        devices.push_back(new Device(id, temp, DeviceType::LOOPBACK));
    }

    HulaAudioSettings *s = HulaAudioSettings::getInstance();

    // outer while gets all the sound cards
    while (snd_card_next(&cardNumber) >= 0 && cardNumber >= 0)
    {
        // open and init the sound card
        cardName = "hw:" + std::to_string(cardNumber);
        snd_ctl_open(&handle, cardName.c_str(), 0);
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
            if (recSet && s->getShowRecordDevices())
            {
                snd_pcm_info_set_stream(subInfo, SND_PCM_STREAM_CAPTURE);
                if (snd_ctl_pcm_info(handle, subInfo) >= 0)
                {
                    DeviceID id;
                    id.linuxID = "hw:" + std::to_string(cardNumber) + "," + std::to_string(subDevice);
                    std::string deviceName = snd_ctl_card_info_get_name(cardInfo);
                    std::string subDeviceName = snd_pcm_info_get_name(subInfo);
                    std::string fullDeviceName = deviceName + ": " + subDeviceName;
                    devices.push_back(new Device(id, fullDeviceName, DeviceType::RECORD));
                }
            }
            if (playSet)
            {
                snd_pcm_info_set_stream(subInfo, SND_PCM_STREAM_PLAYBACK);
                if (snd_ctl_pcm_info(handle, subInfo) >= 0)
                {
                    DeviceID id;
                    id.linuxID = "hw:" + std::to_string(cardNumber) + "," + std::to_string(subDevice);
                    std::string deviceName = snd_ctl_card_info_get_name(cardInfo);
                    std::string subDeviceName = snd_pcm_info_get_name(subInfo);
                    std::string fullDeviceName = deviceName + ": " + subDeviceName;
                    devices.push_back(new Device(id, fullDeviceName, DeviceType::PLAYBACK));
                }
            }
        }
        snd_ctl_close(handle);
    }
    snd_config_update_free_global();
    return devices;
}

/**
 * Override of OSAudio base method. Checks if the selected
 * device is PAVUControl and opens the program if necessary.
 * Control is then passed on to the base method.
 */
bool LinuxAudio::setActiveInputDevice(Device *device)
{
    if (device != nullptr && device->getName() == "Pulse Audio Volume Control")
    {
        std::thread(&LinuxAudio::startPAVUControl).detach();
    }

    return OSAudio::setActiveInputDevice(device);
}

/**
 * Check with the hardware to ensure that the current audio settings
 * are valid for the selected device.
 *
 * @param device Device to check against
 */
bool LinuxAudio::checkDeviceParams(Device *device)
{
    int err;                     // return for commands that might return an error
    snd_pcm_t *pcmHandle = nullptr; // default pcm handle
    snd_pcm_hw_params_t *param;  // defaults param for the pcm
    snd_pcm_format_t format;     // format that user chooses
    unsigned samplingRate;       // sampling rate the user choooses
    bool samplingRateValid;      // bool that gets set if the sampling rate is valid
    bool formatValid;            // bool that gets set if the format is valid

    // device id
    const char *id = device->getID().linuxID.c_str();
    hlDebug() << id << std::endl;
    // open pcm device

    if (device->getType() == DeviceType::PLAYBACK)
    {
        err = snd_pcm_open(&pcmHandle, id, SND_PCM_STREAM_PLAYBACK, 0);
    }
    else
    {
        err = snd_pcm_open(&pcmHandle, id, SND_PCM_STREAM_CAPTURE, 0);
    }

    if (err < 0)
    {
        hlDebug() << "Unable to test device: " << id << std::endl;
        return false;
    }

    // allocate hw params object and fill the pcm device with the default params
    snd_pcm_hw_params_alloca(&param);
    snd_pcm_hw_params_any(pcmHandle, param);

    // test the desired sample rate
    // TODO: insert actual sampling rate
    samplingRate = HulaAudioSettings::getInstance()->getSampleRate();
    samplingRateValid = snd_pcm_hw_params_test_rate(pcmHandle, param, samplingRate, 0) == 0;

    // test the desired format (bit depth)
    format = SND_PCM_FORMAT_FLOAT_LE;
    formatValid = snd_pcm_hw_params_test_format(pcmHandle, param, format) == 0;

    // clean up
    snd_pcm_drain(pcmHandle);
    snd_pcm_close(pcmHandle);
    snd_config_update_free_global();
    if (samplingRateValid && formatValid)
    {
        hlDebug() << HL_SAMPLE_RATE_VALID << std::endl;
        return true;
    }

    if (!samplingRateValid)
    {
        hlDebug() << "Sampling rate was invalid." << std::endl;
    }
    else
    {
        hlDebug() << "Format was invalid." << std::endl;
    }

    hlDebug() << HL_SAMPLE_RATE_INVALID << std::endl;
    throw AudioException(HL_CHECK_PARAMS_CODE, HL_CHECK_PARAMS_MSG);
    return false;
}

/**
 * Open the program Pulse Audio Volume Control to the Record tab.
 * This will allow the user to select the "Monitor of" source
 * that they wish to capture loopback from.
 */
void LinuxAudio::startPAVUControl()
{
    static bool pavuControlOpen = false;
    if (pavuControlOpen)
    {
        return;
    }
    pavuControlOpen = true;
    system("/usr/bin/pavucontrol -t 2");
    pavuControlOpen = false;
}

/*
   lengthOfRecording is in ms
   Device * recordingDevice is already formatted as hw:(int),(int)
   if Device is nullptr then it chooses the default
   */
/**
 * Capture loop for LinuxAudio.
 */
void LinuxAudio::capture()
{
    int err;                        // return for commands that might return an error
    snd_pcm_t *pcmHandle = nullptr;    // default pcm handle
    std::string defaultDevice;      // default hw id for the device
    snd_pcm_hw_params_t *param;     // object to store our paramets (they are just the default ones for now)
    int audioBufferSize;            // size of the buffer for the audio
    uint8_t *audioBuffer = nullptr;       // buffer for the audio
    snd_pcm_uframes_t *temp = nullptr; // useless parameter because the api requires it
    int framesRead = 0;             // amount of frames read

    // just writing to a buffer for now
    defaultDevice = "default";

    // open the pcm device
    err = snd_pcm_open(&pcmHandle, defaultDevice.c_str(), SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0)
    {
        hlDebug() << "Unable to open " << defaultDevice << " exiting..." << std::endl;
        throw AudioException(HL_LINUX_OPEN_DEVICE_CODE, HL_LINUX_OPEN_DEVICE_MSG);
    }

    // allocate hw params object and fill the pcm device with the default params
    snd_pcm_hw_params_alloca(&param);
    snd_pcm_hw_params_any(pcmHandle, param);

    // set to interleaved mode, 16-bit little endian, 2 channels
    snd_pcm_hw_params_set_access(pcmHandle, param, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcmHandle, param, SND_PCM_FORMAT_FLOAT_LE);
    snd_pcm_hw_params_set_channels(pcmHandle, param, 2);

    // we set the sampling rate to whatever the user or device wants
    unsigned int sampleRate = 44100;
    snd_pcm_hw_params_set_rate_near(pcmHandle, param, &sampleRate, nullptr);

    // set the period size to 32 TODO
    snd_pcm_uframes_t frame = FRAME_TIME;
    snd_pcm_hw_params_set_period_size_near(pcmHandle, param, &frame, nullptr);

    // send the param to the the pcm device
    err = snd_pcm_hw_params(pcmHandle, param);
    if (err < 0)
    {
        hlDebug() << "Unable to set parameters: " << defaultDevice << " exiting..." << std::endl;
        throw AudioException(HL_LINUX_SET_PARAMS_CODE, HL_LINUX_SET_PARAMS_MSG);

    }

    // get the size of one period
    snd_pcm_hw_params_get_period_size(param, &frame, nullptr);

    // allocate memory for the buffer
    audioBufferSize = frame * NUM_CHANNELS * sizeof(SAMPLE);
    audioBuffer = (uint8_t *)malloc(audioBufferSize);

    while (!this->endCapture.load())
    {
        // read frames from the pcm
        framesRead = snd_pcm_readi(pcmHandle, audioBuffer, frame);
        if (framesRead == -EPIPE)
        {
            hlDebug() << "Buffer overrun" << std::endl;
            snd_pcm_prepare(pcmHandle);
        }
        else if (framesRead < 0)
        {
            // TODO: This really needs to not be properly switchable and not defaultDevice
            hlDebug() << "ALSA read on device " << defaultDevice << " failed." << std::endl;
        }
        else if (framesRead != (int)frame)
        {
            hlDebug() << "Underrun: Exepected " << frame << " frames but got " << framesRead << std::endl;
        }
        this->copyToBuffers(audioBuffer, framesRead * NUM_CHANNELS * sizeof(SAMPLE));
    }

    // cleanup stuff
    err = snd_pcm_close(pcmHandle);
    if (err < 0)
    {
        hlDebug() << "Unable to close stream." << std::endl;
        throw AudioException(HL_LINUX_ALSA_CLOSE_STREAM_CODE, HL_LINUX_ALSA_CLOSE_STREAM_MSG);
    }
    free(audioBuffer);
}

/**
 * Destructor for LinuxAudio.
 */
LinuxAudio::~LinuxAudio()
{
    hlDebugf("LinuxAudio destructor called\n");

    system("pkill pavucontrol");
}
