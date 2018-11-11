
#include "hlcontrol/internal/HulaSettings.h"

using namespace hula;

HulaSettings *HulaSettings::hlcontrol_instance = nullptr;

/**
 * Private constructor to enforce singleton pattern.
 *
 * Initializes all settings to default values.
 */
HulaSettings::HulaSettings()
{

}

/**
 * Retreive the singular instance of HulaSettings
 * or construct a new one if none exists.
 *
 * @return Pointer to the HulaAudioSettings instance.
 */
HulaSettings * HulaSettings::getInstance()
{
    if (hlcontrol_instance == nullptr)
    {
        hlcontrol_instance = new HulaSettings();
    }

    return hlcontrol_instance;
}

/**
 * Destructor for HulaSettings.
 */
HulaSettings::~HulaSettings()
{
    if (hlcontrol_instance)
    {
        delete hlcontrol_instance;
    }
}