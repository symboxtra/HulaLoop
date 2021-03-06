#include <QLocale>
#include <QTranslator>

#include "hlcontrol/internal/HulaControlError.h"
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

    // Output file
    this->outputFileEncoding = WAV;
}

/**
 * Retreive the singular instance of HulaSettings
 * or construct a new one if none exists.
 *
 * @return Pointer to the HulaSettings instance.
 */
HulaSettings *HulaSettings::getInstance()
{
    if (hlcontrol_instance == nullptr)
    {
        hlcontrol_instance = new HulaSettings();
    }

    return hlcontrol_instance;
}

/**
 * Get the output encoding for the current global
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @return Encoding enum value indicating current encoding type
 */
Encoding HulaSettings::getOutputFileEncoding()
{
    return getInstance()->outputFileEncoding;
}

/**
 * Set the output encoding for the current global
 * configuration. This is the configuration that buffers and
 * output files have been setup for. Changing it mid-anything may
 * break stuff.
 *
 * @param val Encoding enum value indicating current encoding type
 */
void HulaSettings::setOutputFileEncoding(Encoding val)
{
    getInstance()->outputFileEncoding = val;
}

/**
 * Fetch a pointer to the translation object setup for Qt.
 * This translator can be loaded with different languages
 * at runtime to suit the needs of the user.
 *
 * @return QTranslator * Translator object with locale loaded.
 */
QTranslator *HulaSettings::getTranslator()
{
    return trans;
}

/**
 * Remove and delete the current translator, replacing it with
 * a new translator of the specified language.
 *
 * @ref id should be formated as @code lang_country @endcode
 *
 * @param app Reference to the targeted application.
 * @param id Language code and country code of targeted file.
 */
bool HulaSettings::loadLanguage(QCoreApplication *app, const std::string &id)
{
    QTranslator *trans = new QTranslator();
    bool success = trans->load(("hulaloop_" + id).c_str());

    if (success)
    {
        if (this->trans)
        {
            app->removeTranslator(this->trans);
            delete this->trans;
        }

        this->trans = trans;
        app->installTranslator(trans);
    }
    else
    {
        delete trans;
        hlDebug() << "Could not find translation file hulaloop_" << id << "." << std::endl;
    }

    return success;
}

/**
 * Destructor for HulaSettings.
 */
HulaSettings::~HulaSettings()
{
    if (trans)
    {
        delete trans;
    }
}
