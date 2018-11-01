#ifndef HL_EXPORT_H
#define HL_EXPORT_H

#include <hlaudio/hlaudio.h>
#include <string>
#include <vector>

class Export {
    private:
        std::string targetDirectory;

    public:
        Export(std::string targetDirectory);
        void copyData(vector<std::string> dirs);

        static std::string getTempPath();
        static void deleteTempFiles(vector<std::string> dirs);

        ~Export();
};

#endif // HL_EXPORT_H