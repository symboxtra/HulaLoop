#include "hlaudio/internal/HulaAudioSettings.h"

using namespace hula;

HulaAudioSettings *HulaAudioSettings::hlaudio_instance = nullptr;

/**
 * Private constructor to enforce the singleton pattern.
 *
 * Initializes all settings to default values.
 */
HulaAudioSettings::HulaAudioSettings()
{
    // Preferences
    this->showRecordDevices = true;

    // Audio data
    this->numberOfChannels = 2;
    this->sampleRate = 44100;
    this->sampleSize = sizeof(float);

    // Audio devices
    this->defaultInputDeviceName = "";
    this->defaultOutputDeviceName = "";

    // Output file
    this->outputFilePath = "";
    this->outputFileEncoding = WAV;

    // Record
    this->delayTimer = 0;
    this->recordDuration = -1;  // Infinite
}

/**
 * Retreive the singular instance of HulaSettings
 * or construct a new one if none exists.
 *
 * @return Pointer to the HulaAudioSettings instance.
 */
HulaAudioSettings *HulaAudioSettings::getInstance()
{
    if (hlaudio_instance == nullptr)
    {
        hlaudio_instance = new HulaAudioSettings();
    }

    return hlaudio_instance;
}

/**
 * Determine whether or not true record devices (i.e. microphones)
 * should be displayed in device lists.
 *
 * @return Value of showRecordDevices
 */
bool HulaAudioSettings::getShowRecordDevices()
{
    return getInstance()->showRecordDevices;
}

/**
 * Get the number of channels for the current global device
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @return Number of channels that the application is currently configured for
 */
int HulaAudioSettings::getNumberOfChannels()
{
    return getInstance()->numberOfChannels;
}

/**
 * Get the number of channels for the current global device
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @return Sample rate that the application is currentlyy configured for
 */
int HulaAudioSettings::getSampleRate()
{
    return getInstance()->sampleRate;
}

/**
 * Get the sample size for the current global device
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @return Sample size that the application is currently configured for
 */
int HulaAudioSettings::getSampleSize()
{
    return getInstance()->sampleSize;
}

/**
 * Get the name of the device that should be initially selected
 * for record. This defaults to the empty string if not set.
 *
 * @return Name of input device to default to or the empty string if not set
 */
std::string HulaAudioSettings::getDefaultInputDeviceName()
{
    return getInstance()->defaultInputDeviceName;
}

/**
 * Get the name of the device that should be initially selected
 * for playback. This defaults to the empty string if not set.
 *
 * @return Name of the output device to default to or the empty string if not set
 */
std::string HulaAudioSettings::getDefaultOutputDeviceName()
{
    return getInstance()->defaultOutputDeviceName;
}

/**
 * Get the path of the output file specified at startup or during
 * the file save routine. This defaults to the empty string if not set.
 *
 * @return Path to output file
 */
std::string HulaAudioSettings::getOutputFilePath()
{
    return getInstance()->outputFilePath;
}

/**
 * Get the output encoding for the current global
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @return Encoding enum value indicating current encoding type
 */
Encoding HulaAudioSettings::getOutputFileEncoding()
{
    return getInstance()->outputFileEncoding;
}

/**
 * Get the length of the delay timer in seconds.
 *
 * @return Length of the delay timer in seconds
 */
double HulaAudioSettings::getDelayTimer()
{
    return getInstance()->delayTimer;
}

/**
 * Get the length of the record in seconds.
 * This will be -1 for infinite record.
 *
 * @return Length of record in seconds
 */
double HulaAudioSettings::getRecordDuration()
{
    return getInstance()->recordDuration;
}

/**
 * Set whether or not true record devices (i.e. microphones)
 * should be displayed in the device lists.
 *
 * @param val Value of option.
 */
void HulaAudioSettings::setShowRecordDevices(bool val)
{
    getInstance()->showRecordDevices = val;
}

/**
 * Set the number of channels for the current global device
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @param val Number of channels that the application is currently configured for
 */
void HulaAudioSettings::setNumberOfChannels(int val)
{
    getInstance()->numberOfChannels = val;
}

/**
 * Set the number of channels for the current global device
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @param val Sample rate that the application is currentlyy configured for
 */
void HulaAudioSettings::setSampleRate(int val)
{
    getInstance()->sampleRate = val;
}

/**
 * Set the sample size for the current global device
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @param val Sample size that the application is currently configured for
 */
void HulaAudioSettings::setSampleSize(int val)
{
    getInstance()->sampleSize = val;
}

/**
 * Set the name of the device that should be initially selected
 * for record.
 *
 * @param val Name of input device to default to or the empty string if not set
 */
void HulaAudioSettings::setDefaultInputDeviceName(std::string val)
{
    getInstance()->defaultInputDeviceName = val;
}

/**
 * Set the name of the device that should be initially selected
 * for playback.
 *
 * @param val Name of the output device to default to or the empty string if not set
 */
void HulaAudioSettings::setDefaultOutputDeviceName(std::string val)
{
    getInstance()->defaultOutputDeviceName = val;
}

/**
 * Set the path of the output file specified at startup or during
 * the file save routine.
 *
 * @param val Path to output file
 */
void HulaAudioSettings::setOutputFilePath(std::string val)
{
    getInstance()->outputFilePath = val;
}

/**
 * Set the output encoding for the current global
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @param val Encoding enum value indicating current encoding type
 */
void HulaAudioSettings::setOutputFileEncoding(Encoding val)
{
    getInstance()->outputFileEncoding = val;
}

/**
 * Set the length of the delay timer in seconds.
 *
 * @param val Length of the delay timer in seconds
 */
void HulaAudioSettings::setDelayTimer(double val)
{
    getInstance()->delayTimer = val;
}

/**
 * Get the length of the record in seconds.
 * Use -1 for infinite record.
 *
 * @param val Length of record in seconds
 */
void HulaAudioSettings::setRecordDuration(double val)
{
    getInstance()->recordDuration = val;
}

/**
 * Destructor for HulaAudioSettings.
 */
HulaAudioSettings::~HulaAudioSettings()
{
    if (hlaudio_instance)
    {
        delete hlaudio_instance;
    }
}
