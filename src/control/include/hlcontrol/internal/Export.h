#ifndef HL_EXPORT_H
#define HL_EXPORT_H

#include <hlaudio/hlaudio.h>
#include <string>

namespace hula
{
    class Export {
        private:
            std::string targetFile;

        public:
            Export(std::string targetFile);
            void copyData(std::string inputFileDirectory);

            ~Export();
    };
}

#endif // HL_EXPORT_H