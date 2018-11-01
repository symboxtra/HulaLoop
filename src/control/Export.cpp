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
    // Initialize libsndfile info.
    SF_INFO sfinfo;
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.channels = NUM_CHANNELS;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    float* buffer = new float[512];

    SNDFILE *out_file = sf_open(this->targetDirectory.c_str(), SFM_WRITE, &sfinfo);

    for(int i = 0;i < dirs.size();i++)
    {
        // opens the files
        SNDFILE* in_file = sf_open(dirs[i].c_str(), SFM_READ, &sfinfo);

        while(true)
        {
            sf_count_t framesRead = sf_readf_float(in_file, buffer, 512 / NUM_CHANNELS);
            sf_count_t framesWritten = sf_writef_float(out_file, buffer, framesRead);

            printf("Frames Read: %f\nFrames written: %f\n", framesRead, framesWritten);

            if(framesRead != 512 / NUM_CHANNELS)
                break;
        }

        // Close the file
        sf_close(in_file);
    }

    sf_close(out_file);
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