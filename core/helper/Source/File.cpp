//
// Created by rxwyun on 2021/8/24.
//

#include "../Include/File.h"
#include <glob.h>
#include <unistd.h>
#include <cstring>

void ArmFaceIdentify::File::write(const string &filePath, const string& content, unsigned int mode) {
    ofstream outfile(filePath, mode);
    if (!outfile.is_open()) {
        throw string("file ").append(filePath).append(" can not be open!");
    }

    outfile << content;

    outfile.close();
}

string ArmFaceIdentify::File::read(const string &filePath, unsigned int mode) {
    ifstream infile(filePath, mode);
    string content;
    infile >> content;

    infile.close();

    return content;
}

vector<string> ArmFaceIdentify::File::glob(const string &pattern) {
    glob_t pglob;
    if (::glob(pattern.c_str(), GLOB_ERR, NULL, &pglob) != 0){
        throw string("pattern ").append(pattern).append(" error!");
    }

    vector<string> files;
    for (int i = 0; i < pglob.gl_pathc; i++){
        files.push_back(pglob.gl_pathv[i]);
    }
    globfree(&pglob);

    return files;
}

void ArmFaceIdentify::File::unlink(const string &filePath) {
    try {
        ::unlink(filePath.c_str());
    } catch (exception e) {
        ;
    }
}

bool ArmFaceIdentify::File::exists(const string &filePath) {
    return access(filePath.c_str(), W_OK|R_OK) == 0;
}

void ArmFaceIdentify::File::mkdirs(const string &dir) {
    int i,len;
    char str[512];
    strncpy(str, dir.c_str(), 512);
    len = strlen(str);
    for(i=0; i < len; i++) {
        if( str[i]=='/' ) {
            str[i] = '\0';
            if(access(str,0)!=0){
                mkdir(str, 0777);
            }
            str[i]='/';
        }
    }
    if( len>0 && access(str,0)!=0 ){
        mkdir(str, 0777);
    }
}
