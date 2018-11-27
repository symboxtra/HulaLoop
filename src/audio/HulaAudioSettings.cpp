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
 * Destructor for HulaAudioSettings.
 */
HulaAudioSettings::~HulaAudioSettings()
{
    if (hlaudio_instance)
    {
        delete hlaudio_instance;
    }
}
