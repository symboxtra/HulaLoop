#include "hlcontrol/internal/Export.h"

#include <fstream>
#include <string>

using namespace hula;

/**
 * Construct a new instance of the Export class.
 *
 * @param The target directory of the file
 */
Export::Export(std::string targetFile)
{
    this->targetFile = targetFile;
}

/**
 * Copy data from the temp file to the output file.
 *
 * @param The input file directory to copy from
 */
void Export::copyData(std::string inputFileDirectory)
{
    // Open the files
    std::ifstream iFile(inputFileDirectory, std::ios::binary);
    if (iFile.fail())
    {
        // TODO: Handle error
        std::cerr << "Error opening file: " << inputFileDirectory << std::endl;
    }
    std::ofstream oFile(this->targetFile, std::ios::binary);
    if (oFile.fail())
    {
        // TODO: Handle error
        std::cerr << "Error opening file: " << this->targetFile << std::endl;
    }

    // copies the file
    oFile << iFile.rdbuf();

    // close the file
    oFile.close();
    iFile.close();
}

/**
 * Destroys the instance of Export
 */
Export::~Export()
{

}