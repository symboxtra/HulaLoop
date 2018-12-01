#include <portaudio.h>

#include <fcntl.h>
#include <libgen.h>
#include <libproc.h>
#include <signal.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <thread>

#include "OSXAudio.h"
#include "hlaudio/internal/HulaAudioError.h"
#include "hlaudio/internal/HulaAudioSettings.h"

using namespace hula;

/**
 * Constructs an instance of OSXAudio class.
 *
 * This will initialize an OSXDaemon which will handle the virtual loopback.
 * OSXDaemon will join or start a JACK server to get audio.
 */
OSXAudio::OSXAudio()
{
    // Create the loopback daemon
    int pid = isDaemonRunning();
    if (pid > 0)
    {
        hlDebugf("OSXDaemon already running (PID: %d)\n", pid);
    }
    else
    {
        hlDebugf("OSXDaemon was not running. Starting now...\n");
        startDaemon();
    }

    // Initializing PortAudio produces a crap ton of output
    // Shut it up unless this is a debug build
    // TODO: This should really be determined at runtime not compile-time
    #if HL_NO_DEBUG_OUTPUT
    int out = dup(1);
    int temp_null = open("/dev/null", O_WRONLY);
    dup2(temp_null, 1);
    close(temp_null);
    #endif

    // Initialize PortAudio
    int err = Pa_Initialize();
    if (err != paNoError)
    {
        hlDebugf("PortAudio failed to initialize.\n");
        hlDebugf("PortAudio: %s\n", Pa_GetErrorText(err));
        exit(1); // TODO: Handle error
    }

    #if HL_NO_DEBUG_OUTPUT
    dup2(out, 1);
    close(out);
    #endif
}

/**
 * Check if the @ref OSXDaemon is already running using pgrep.
 *
 * @return int PID of running daemon. -1 if not running
 */
int OSXAudio::isDaemonRunning()
{
    int ret;
    int procPipe[2];

    pipe(procPipe);
    ret = fork();

    // Child process
    if (ret == 0)
    {
        // Close the input since we don't need to write to the process
        close(procPipe[0]);

        // Redirect the process's output
        dup2(procPipe[1], 1);
        close(procPipe[1]);

        std::string pgrep = "/usr/bin/pgrep";
        ret = execlp(pgrep.c_str(), pgrep.c_str(), "hulaloop-osx-daemon", nullptr);

        hlDebugf("Failed to start pgrep process.\n");
        hlDebugf("Execution of %s failed with return code: %d\n", pgrep.c_str(), ret);
        perror("ERRNO");
        exit(EXIT_FAILURE);
    }
    // Failure
    else if (ret < 0)
    {
        hlDebugf("Failed to start pgrep process.\nFork failed.\n");
        exit(EXIT_FAILURE);
    }
    // Parent process
    else
    {
        // Close the output since we don't need to write to the process
        close(procPipe[1]);

        // Read from the pipe
        char c;
        std::string buffer = "";
        while (read(procPipe[0], &c, 1) > 0)
        {
            // Only grab the first PID
            if (c == '\n')
            {
                break;
            }

            buffer += c;
        }

        close(procPipe[0]);

        pid_t pid = -1;
        if (buffer.size() > 0)
        {
            hlDebugf("Parsed PID: %s\n", buffer.c_str());
            try
            {
                pid = std::stoi(buffer);
            }
            catch (std::invalid_argument &e)
            {
                (void)e;
                hlDebugf("Failed to convert parsed PID.\n");
            }
        }

        return pid;
    }
}

/**
 * Start the @ref OSXDaemon to transfer audio from the CoreAudio driver
 * to the JACK client. This expects hulaloop-osx-daemon to be in the same
 * directory as hulaloop.
 */
pid_t OSXAudio::startDaemon()
{
    pid_t ret;
    pid_t pid;
    char path[PROC_PIDPATHINFO_MAXSIZE];

    // Get the path to the running hulaloop instance
    pid = getpid();
    ret = proc_pidpath(pid, path, PROC_PIDPATHINFO_MAXSIZE);
    if (ret <= 0)
    {
        hlDebugf("proc_pidpath failed with return of %d.\n", ret);
        hlDebugf("Could not get path to current executable.\n");
        exit(1);
    }

    // Get the directory
    char *installDir = dirname(path);
    if (installDir == nullptr)
    {
        hlDebugf("Could not trim executable name from install path. Error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    std::string executable = std::string(installDir);
    executable += "/hulaloop-osx-daemon";

    ret = fork();

    // Child process
    if (ret == 0)
    {
        ret = execlp(executable.c_str(), executable.c_str(), nullptr);

        hlDebugf("Failed to start hulaloop-osx-daemon process.\n");
        hlDebugf("Execution of %s failed with return code: %d\n", executable.c_str(), ret);
        perror("ERRNO");
        exit(EXIT_FAILURE);
    }
    // Failure
    else if (ret < 0)
    {
        hlDebugf("Failed to start hulaloop-osx-daemon process.\nFork failed.\n");
        exit(EXIT_FAILURE);
    }
    // Parent process
    else
    {
        // Wait for the spawn process to exit
        // This will indicate that the daemon has taken over
        int proc_ret;
        waitpid(ret, &proc_ret, 0);

        // Get the real PID of the daemon
        pid_t success = isDaemonRunning();
        if (success > 0)
        {
            hlDebugf("hulaloop-osx-daemon started (PID: %d)\n", success);
        }
        else
        {
            hlDebugf("hulaloop-osx-daemon crashed.\n");
            exit(EXIT_FAILURE);
        }

        return success;
    }
}

/**
 * Restart the @ref OSXDaemon.
 */
pid_t OSXAudio::restartDaemon()
{
    // Kill all instances
    pid_t pid;
    while ((pid = isDaemonRunning()) > 0)
    {
        kill(pid, SIGTERM);
    }

    return startDaemon();
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
    OSXAudio *obj = (OSXAudio *)userData;
    float *samples = (float *)inputBuffer;

    // Prevent unused variable warnings.
    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    // TODO: Make sure this calculation is right
    obj->copyToBuffers(samples, framesPerBuffer * NUM_CHANNELS);

    return paContinue;
}

/**
 * Execution loop for loopback capture
 *
 * Based on code from https://app.assembla.com/spaces/portaudio/git/source/master/examples/paex_record_file.c
 */
void OSXAudio::capture()
{
    PaStreamParameters  inputParameters = {0};
    PaStream           *stream;
    PaError             err = paNoError;

    inputParameters.device = this->activeInputDevice->getID().portAudioID;
    if (inputParameters.device == paNoDevice)
    {
        hlDebug() << "Device %s not found." << this->activeInputDevice->getName() << std::endl;
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
              nullptr,                  // &outputParameters
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,             // We won't output out of range samples so don't bother clipping them
              paRecordCallback,
              this                   // Pass our instance in
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

    hlDebugf("Capture thread keep-alive started...\n");
    while (!this->endCapture)
    {
        // Keep this thread alive
        // The second half of this function could be moved to a separate
        // function like endCapture() so that we don't have to keep this thread alive.
        Pa_Sleep(20);
    }

    hlDebugf("Capture thread ended keep-alive.\n");

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
std::vector<Device *> OSXAudio::getDevices(DeviceType type)
{
    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0)
    {
        hlDebugf("Failed to fetch PortAudio devices.\n");
        exit(1); // TODO: Handle error
    }

    HulaAudioSettings *s = HulaAudioSettings::getInstance();

    std::vector<Device *> devices;
    for (uint32_t i = 0; i < deviceCount; i++)
    {
        const PaDeviceInfo *paDevice = Pa_GetDeviceInfo(i);
        DeviceType checkType = (DeviceType) 0;

        // We can only support OSX loopback on our own driver
        if (strcmp(paDevice->name, "HulaLoop") == 0)
        {
            checkType = (DeviceType)(checkType | DeviceType::LOOPBACK);
        }
        if (paDevice->maxOutputChannels > 0)
        {
            checkType = (DeviceType)(checkType | DeviceType::PLAYBACK);
        }
        if (paDevice->maxInputChannels > 0)
        {
            checkType = (DeviceType)(checkType | DeviceType::RECORD);
        }

        // Create HulaLoop style device and add to vector
        // This needs to be freed elsewhere
        if (type & checkType)
        {
            if (checkType == DeviceType::RECORD && !s->getShowRecordDevices())
            {
                continue;
            }

            DeviceID id;
            id.portAudioID = i;
            Device *hlDevice = new Device(id, std::string(paDevice->name), checkType);
            devices.push_back(hlDevice);
        }
    }

    return devices;
}

/**
 * Checks the sampling rate and bit depth of the device
 *
 * @param device Instance of Device that corresponds to the desired system device
 */
bool OSXAudio::checkDeviceParams(Device *device)
{
    PaStreamParameters inputParameters = {0};
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.device = device->getID().portAudioID;
    inputParameters.sampleFormat = paFloat32;

    PaError err = Pa_IsFormatSupported(&inputParameters, nullptr, HulaAudioSettings::getInstance()->getSampleRate());

    if (err == paFormatIsSupported)
    {
        hlDebug() << HL_SAMPLE_RATE_VALID << std::endl;
    }
    else
    {
        hlDebug() << HL_SAMPLE_RATE_INVALID << std::endl;
    }

    return err == paFormatIsSupported;
}

/**
 * Deconstructs the OSXAudio instance
 */
OSXAudio::~OSXAudio()
{
    hlDebugf("OSXAudio destructor called\n");

    // Close the Port Audio session
    PaError err = Pa_Terminate();
    if (err != paNoError)
    {
        hlDebugf("Could not terminate Port Audio session.\n");
        hlDebugf("PortAudio: %s\n", Pa_GetErrorText(err));
    }
}
