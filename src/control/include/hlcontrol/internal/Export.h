#ifndef HL_EXPORT_H
#define HL_EXPORT_H

#include <hlaudio/hlaudio.h>
#include <string>
using namespace std;

class Export {
    private:
        string targetDirectory;

    public:
        Export(string targetDirectory);
        void copyData(string inputFileDirectory);

        ~Export();
};

#endif // HL_EXPORT_H