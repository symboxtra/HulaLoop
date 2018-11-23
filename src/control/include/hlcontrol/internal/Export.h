#ifndef HL_EXPORT_H
#define HL_EXPORT_H

#include <hlaudio/hlaudio.h>
#include <string>
#include <vector>

namespace hula
{
    class Export
    {
        private:
            std::string targetFile;

        public:
            Export(std::string targetFile);
            void copyData(std::vector<std::string> dirs);

            std::string getFileExtension(std::string file_path);

            static std::string getTempPath();
            static void deleteTempFiles(std::vector<std::string> dirs);

            ~Export();
    };
} // namespace hula

#endif // HL_EXPORT_H