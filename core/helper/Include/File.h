//
// Created by rxwyun on 2021/8/24.
//

#ifndef ARM_FACE_IDENTIFY_FILE_H
#define ARM_FACE_IDENTIFY_FILE_H

#include <sstream>
#include <fstream>

using namespace std;

namespace ArmFaceIdentify {
    class File {
    public:
        static void write(const string& filePath, const string& content, unsigned int openmode = ios::trunc|ios::out);
    };
}


#endif //ARM_FACE_IDENTIFY_FILE_H
