#ifndef HL_SETTINGS_H
#define HL_SETTINGS_H

#include <QCoreApplication>
#include <QTranslator>

#include <hlaudio/internal/HulaAudioSettings.h>

namespace hula
{
    /**
     * Specifiy the encoding type for the output file.
     */
    enum Encoding
    {
        WAV, FLAC, CAF, AIFF
    };

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

            Encoding outputFileEncoding;

        protected:
            QTranslator *trans;
            HulaSettings();

        public:
            static HulaSettings *getInstance();

            void setOutputFileEncoding(Encoding);
            Encoding getOutputFileEncoding();

            QTranslator *getTranslator();
            bool loadLanguage(QCoreApplication *app, const std::string &id);

            ~HulaSettings();


    };
}

#endif // END HL_SETTINGS_H