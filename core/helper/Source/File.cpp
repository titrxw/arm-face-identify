//
// Created by rxwyun on 2021/8/24.
//

#include "../Include/File.h"

void ArmFaceIdentify::File::write(const string &filePath, const string& content, unsigned int openmode) {
    ofstream outfile(filePath, openmode);
    if (!outfile.is_open()) {
        return;
    }

    outfile << content;

    outfile.close();
}
