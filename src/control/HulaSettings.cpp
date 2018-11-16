#include <QLocale>
#include <QTranslator>

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
    trans = new QTranslator();
    trans->load(QLocale(), "hulaloop", "_");
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
 * Fetch a pointer to the translation object setup for Qt.
 * This translator can be loaded with different languages
 * at runtime to suit the needs of the user.
 *
 * @return QTranslator * Translator object with locale loaded.
 */
QTranslator * HulaSettings::getTranslator()
{
    return trans;
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