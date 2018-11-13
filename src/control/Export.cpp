#include "hlcontrol/internal/Export.h"

#include <cstdio>
#include <fstream>
#include <string>

#include <QDir>
#include <sndfile.h>

/**
 * Construct a new instance of the Export class.
 *
 * @param The target directory of the file
 */
Export::Export(std::string targetDirectory)
{
    this->targetDirectory = targetDirectory;
}

/**
 * Gets the system temporary directory
 *
 * @return string the absolute path to the system temp directory
 */
std::string Export::getTempPath()
{
    return QDir::toNativeSeparators(QDir::tempPath()).toStdString();
}

/**
 * Copies the data from the temp file
 *
 * @param The input file directory to copy from
 */
void Export::copyData(vector<std::string> dirs)
{

    // Get file extension of the target export file
    std::string extension = getFileExtension(this->targetDirectory.c_str()); // TODO: Return encoding enum value from HulaAudioSettings

    std::cout << extension << std::endl;

    // Initialize libsndfile info.
    SF_INFO sfinfo_in;
    sfinfo_in.samplerate = SAMPLE_RATE;
    sfinfo_in.channels = NUM_CHANNELS;
    sfinfo_in.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    // Initialize libsndfile info.
    SF_INFO sfinfo_out;
    sfinfo_out.samplerate = SAMPLE_RATE;
    sfinfo_out.channels = NUM_CHANNELS;
    sfinfo_out.format = SF_FORMAT_FLOAT;
    float* buffer = new float[512];

    // Set libsndfile settings based on extension
    // TODO: Compare against Encoding enum from HulaAudioSettings
    if (!extension.compare("wav"))
        sfinfo_out.format |= SF_FORMAT_WAV;
    else if (!extension.compare("flac"))
        sfinfo_out.format |= SF_FORMAT_FLAC;
    else if(!extension.compare("caf"))
        sfinfo_out.format |= SF_FORMAT_CAF;
    else if(!extension.compare("aiff"))
        sfinfo_out.format |= SF_FORMAT_AIFF;

    SNDFILE *out_file = sf_open(this->targetDirectory.c_str(), SFM_WRITE, &sfinfo_out);

    for(int i = 0;i < dirs.size();i++)
    {
        // opens the files
        SNDFILE* in_file = sf_open(dirs[i].c_str(), SFM_READ, &sfinfo_in);

        while(true)
        {
            sf_count_t framesRead = sf_readf_float(in_file, buffer, 512 / NUM_CHANNELS);
            sf_count_t framesWritten = sf_writef_float(out_file, buffer, framesRead);

            //printf("Frames Read: %f\nFrames written: %f\n", framesRead, framesWritten);

            if(framesRead != 512 / NUM_CHANNELS)
                break;
        }

        // Close the file
        sf_close(in_file);
    }

    sf_close(out_file);
}

/**
 * @brief Identify the file extension of the provided file
 *
 * @param file_path path to the file to identify the extension
 * @return std::string the extension of the provided file
 */
std::string Export::getFileExtension(std::string file_path)
{
    size_t found = file_path.find_last_of(".");

    if(found == std::string::npos)
        return "";
    else
    {
        return file_path.substr(found + 1);
    }
}

/**
 * Deletes all the files in the vector
 */
void Export::deleteTempFiles(vector<std::string> dirs)
{
    // loop throught all the files
    for(std::string file : dirs)
    {
        // no good c++ function so we'll just use the C one
        remove((char *)file.c_str());
    }
}

/**
 * Destroys the instance of Export
 */
Export::~Export()
{

}