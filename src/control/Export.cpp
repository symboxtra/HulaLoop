#include "hlcontrol/internal/Export.h"

#include <fstream>
#include <string>

#include <QDir>

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
    // Open target file
    ofstream oFile(this->targetDirectory, ios::binary);
    if (oFile.fail())
    {
        // TODO: Handle error
        cerr << "Error opening file: " << this->targetDirectory << endl;
    }

    cout << "Size: " << dirs.size() << endl;

    for(int i = 0;i < dirs.size();i++)
    {
        // opens the files
        ifstream iFile(dirs[i].c_str(), ios::binary);
        if (iFile.fail())
        {
            // TODO: Handle error
            cerr << "Error opening file: " << dirs[i] << endl;
        }
        // Copies the file
        oFile << iFile.rdbuf();

        // Close the file
        iFile.close();
    }

    oFile.close();
}

/**
 * Destroys the instance of Export
 */
Export::~Export()
{

}