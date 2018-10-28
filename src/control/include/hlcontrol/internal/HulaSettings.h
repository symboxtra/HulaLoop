#ifndef HULA_SETTINGS_H
#define HULA_SETTINGS_H

#include <hlaudio/internal/HulaAudioSettings.h>

/**
 * Singleton class containing all settings for the application.
 * This includes audio specific settings.
 */
class HulaSettings : public HulaAudioSettings {

    private:
        /**
         * Private instance.
         */
        static HulaSettings *hlcontrol_instance;

    protected:
        HulaSettings();

    public:
        static HulaSettings * getInstance();

        ~HulaSettings();


};

#endif // END HULA_SETTINGS_H