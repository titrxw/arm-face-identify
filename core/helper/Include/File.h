//
// Created by rxwyun on 2021/8/24.
//

#ifndef ARM_FACE_IDENTIFY_FILE_H
#define ARM_FACE_IDENTIFY_FILE_H

#include <sstream>
#include <fstream>
#include <vector>
#include <sys/stat.h>

using namespace std;

namespace ArmFaceIdentify {
    class File {
    public:
        static void write(const string& filePath, const string& content, ios_base::openmode mode = ios::trunc|ios::out);
        static string read(const string& filePath, ios_base::openmode mode = ios::out|ios::in);
        static vector<string> glob(const string &pattern);
        static void unlink(const string &filePath);
        static bool exists(const string &filePath);
        static void mkdirs(const string &dir);
    };
}


#endif //ARM_FACE_IDENTIFY_FILE_H
