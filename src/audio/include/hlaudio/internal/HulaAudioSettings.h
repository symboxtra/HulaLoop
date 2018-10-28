#ifndef HULA_AUDIO_SETTINGS_H
#define HULA_AUDIO_SETTINGS_H

#include <string>

/**
 * Specifiy the encoding type for the output file.
 *
 * Temporarily defined here until the Encoder class is built.
 */
enum Encoding {
    WAV, MP3
};

/**
 * Class containing all settings pertinent to the audio module.
 */
class HulaAudioSettings {
    private:
        /**
         * Private instance.
         */
        static HulaAudioSettings *hlaudio_instance;

        bool showRecordDevices;

        int numberOfChannels;
        int sampleRate;
        int sampleSize;

        std::string defaultInputDeviceName;
        std::string defaultOutputDeviceName;

        std::string outputFilePath;
        Encoding outputFileEncoding;

        double delayTimer;
        double recordDuration;

    protected:
        HulaAudioSettings();

    public:
        static HulaAudioSettings * getInstance();

        /**
         * Getters
         */

        bool getShowRecordDevices();

        int getNumberOfChannels();
        int getSampleRate();
        int getSampleSize();

        std::string getDefaultInputDeviceName();
        std::string getDefaultOutputDeviceName();

        std::string getOutputFilePath();
        Encoding getOutputFileEncoding();

        double getDelayTimer();
        double getRecordDuration();

        /**
         * Setters
         */

        void setShowRecordDevices(bool);

        void setNumberOfChannels(int);
        void setSampleRate(int);
        void setSampleSize(int);

        void setDefaultInputDeviceName(std::string);
        void setDefaultOutputDeviceName(std::string);

        void setOutputFilePath(std::string);
        void setOutputFileEncoding(Encoding);

        void setDelayTimer(double);
        void setRecordDuration(double);

        ~HulaAudioSettings();
};

#endif // END HULA_AUDIO_SETTINGS_H
