#ifndef HL_EXPORT_H
#define HL_EXPORT_H

#include <hlaudio/hlaudio.h>
#include <string>
#include <vector>

namespace hula
{
    /**
     * An class used to copy data from temp files and export files
    */
    class Export {
        private:
            std::string targetFile;

        public:
            Export(std::string targetFile);
            void copyData(std::vector<std::string> dirs);

            static std::string getTempPath();
            static void deleteTempFiles(std::vector<std::string> dirs);

            ~Export();
    };
}

#endif // HL_EXPORT_H