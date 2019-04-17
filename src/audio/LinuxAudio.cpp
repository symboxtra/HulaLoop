#include <fcntl.h>
#include <unistd.h>
#include <pulse/simple.h>
#include <pulse/error.h>

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

    // check what devices we need to get
    bool loopSet = (type & DeviceType::LOOPBACK) == DeviceType::LOOPBACK;
    bool recSet = (type & DeviceType::RECORD) == DeviceType::RECORD;
    bool playSet = (type & DeviceType::PLAYBACK) == DeviceType::PLAYBACK;

    HulaAudioSettings *s = HulaAudioSettings::getInstance();

    // Fetch from pactl
    if (loopSet || recSet || playSet)
    {
        std::vector<Device *> newDevices = parsePulseAudioDevices();

        for (int i = 0; i < newDevices.size(); i++)
        {
            if (loopSet && newDevices[i]->getType() == DeviceType::LOOPBACK)
            {
                devices.push_back(newDevices[i]);
            }
            else if (recSet && s->getShowRecordDevices() && newDevices[i]->getType() == DeviceType::RECORD)
            {
                devices.push_back(newDevices[i]);
            }
            else if (playSet && newDevices[i]->getType() == DeviceType::PLAYBACK)
            {
                devices.push_back(newDevices[i]);
            }
        }
    }

    return devices;
}

/**
 * Helper function to open a pipe, run a command,
 * and return the output.
 *
 * Only works on POSIX compliant systems.
 */
std::string pipeAndReadOutput(const char *command)
{
    std::string output = "";

    // Open a pipe
    FILE *pipe = popen(command, "r");
    if (!pipe)
    {
        hlDebug() << "Could not open pipe to execute command: " << command << std::endl;
        return "";
    }

    char c;
    while (!feof(pipe) && (c = fgetc(pipe)))
    {
        output += c;
    }

    pclose(pipe);

    return output;
}

/**
 * Call pactl list and parse the PulseAudio devices from it.
 *
 * This should eventually be replaced by the introspection abilities of
 * the PulseAudio async API. Since we're using the simple API, this will
 * do for now.
 */
std::vector<Device *> LinuxAudio::parsePulseAudioDevices()
{
    // If keywords are changed, must update type conditions below
    std::vector<std::string> keywords = {"Source #", "Sink #"};
    std::vector<Device *> devices;
    std::string output;
    size_t pos;
    size_t endPos;

    output = pipeAndReadOutput("pactl list");

    for (int i = 0; i < keywords.size(); i++)
    {
        endPos = 0;

        // Split output by keyword
        while ((pos = output.find(keywords[i], endPos)) != std::string::npos)
        {
            DeviceID id;
            std::string name;
            DeviceType type;
            std::string sub;
            size_t subPos;
            size_t subEndPos;

            // Search for next identifier to split at
            endPos = output.find(keywords[i], pos + 1);
            if (endPos == std::string::npos)
                sub = output.substr(pos);
            else
                sub = output.substr(pos, endPos - pos);

            // Collect id
            subPos = sub.find("Name: ") + 6;
            subEndPos = sub.find("\n", subPos);
            id.linuxID = sub.substr(subPos, subEndPos - subPos);

            // Collect name
            // TODO: Could decide to strip "Monitor of"
            subPos = sub.find("Description: ") + 13;
            subEndPos = sub.find("\n", subPos);
            name = sub.substr(subPos, subEndPos - subPos);

            if (i == 0 && id.linuxID.find(".monitor", id.linuxID.length() - 8) != std::string::npos)
            {
                type = DeviceType::LOOPBACK;
            }
            else if (i == 0)
            {
                type = DeviceType::RECORD;
            }
            else if (i == 1)
            {
                type = DeviceType::PLAYBACK;
            }

            hlDebug() << "Creating device from pactl:" << std::endl;
            hlDebug() << "    ID: " << id.linuxID << std::endl;
            hlDebug() << "    Name: " << name << std::endl;

            devices.push_back(new Device(id, name, type));
        }
    }

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
    int err = 0;

    hlDebug() << "Testing PulseAudio device: " << device->getID().linuxID << std::endl;

    // PulseAudio variables
    pa_simple *s;
    pa_sample_spec ss;
    std::string deviceName = device->getID().linuxID;

    ss.format = PA_SAMPLE_FLOAT32;
    ss.channels = NUM_CHANNELS;
    ss.rate = HulaAudioSettings::getInstance()->getSampleRate();

    if (device->getType() == DeviceType::PLAYBACK)
    {
        s = pa_simple_new(
            NULL,
            "HulaLoop",
            PA_STREAM_PLAYBACK,
            deviceName.c_str(),
            "HulaLoop Device Check",
            &ss,
            NULL,
            NULL,
            NULL
        );
    }
    else
    {
        s = pa_simple_new(
            NULL,
            "HulaLoop",
            PA_STREAM_RECORD,
            deviceName.c_str(),
            "HulaLoop Device Check",
            &ss,
            NULL,
            NULL,
            NULL
        );
    }

    if (!s)
    {
        hlDebug() << "" << std::endl;
        throw AudioException(HL_CHECK_PARAMS_CODE, HL_CHECK_PARAMS_MSG);
        return false;
    }

    return true;
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
    int err = 0, ret = 0;           // return for commands that might return an error
    int audioBufferSize;            // size of the buffer for the audio
    float *audioBuffer = nullptr;   // buffer for the audio

    // PulseAudio variables
    pa_simple *s;
    pa_sample_spec ss;
    std::string deviceName;

    ss.format = PA_SAMPLE_FLOAT32;
    ss.channels = NUM_CHANNELS;
    ss.rate = HulaAudioSettings::getInstance()->getSampleRate();

    // Allocate memory for the buffer
    audioBufferSize = HL_LINUX_FRAMES_PER_BUFFER * NUM_CHANNELS * sizeof(SAMPLE);
    audioBuffer = new float[audioBufferSize];

    // Grab device name
    deviceName = this->activeInputDevice->getID().linuxID;

    s = pa_simple_new(
        NULL,
        "HulaLoop",
        PA_STREAM_RECORD,
        deviceName.c_str(),
        "HulaLoop Record",
        &ss,
        NULL,
        NULL,
        NULL
    );

    if (!s)
    {
        hlDebug() << "Could not open PulseAudio stream to " << deviceName << std::endl;
        pa_simple_free(s);

        throw AudioException(HL_LINUX_OPEN_DEVICE_CODE, HL_LINUX_OPEN_DEVICE_MSG);
    }

    while (!this->endCapture.load())
    {
        // This will block until bytes are available
        ret = pa_simple_read(s, (void *)audioBuffer, audioBufferSize, &err);

        if (ret < 0)
        {
            hlDebugf("PulseAudio read on device %s failed: %s\n", deviceName.c_str(), pa_strerror(err));
            // TODO: cleanup and throw error?
        }

        copyToBuffers((float *)audioBuffer, HL_LINUX_FRAMES_PER_BUFFER * NUM_CHANNELS);
        doCallbacks((float *)audioBuffer, HL_LINUX_FRAMES_PER_BUFFER * NUM_CHANNELS);
    }

    // cleanup stuff
    if (s)
    {
        pa_simple_free(s);
        hlDebug() << "Freed PulseAudio stream." << std::endl;
    }

    delete [] audioBuffer;
}

/**
 * Destructor for LinuxAudio.
 */
LinuxAudio::~LinuxAudio()
{
    hlDebugf("LinuxAudio destructor called\n");

    system("pkill pavucontrol");
}
