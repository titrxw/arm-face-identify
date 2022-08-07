//
// Created by rxw on 2022/8/1.
//

#ifndef ARM_FACE_IDENTIFY_FILESYSTEM_HPP
#define ARM_FACE_IDENTIFY_FILESYSTEM_HPP

#include <unistd.h>
#include <sys/stat.h>
#include <glob.h>

class Filesystem {
public:
    string static getCurUserHomeDir() {
        string path = "~";
        char const* home = getenv("HOME");
        if (home or ((home = getenv("USERPROFILE")))) {
            path.replace(0, 1, home);
        } else {
            char const *hdrive = getenv("HOMEDRIVE"),
            *hpath = getenv("HOMEPATH");
            assert(hdrive);  // or other error handling
            assert(hpath);
            path.replace(0, 1, std::string(hdrive) + hpath);
        }

        return path;
    }

    string static getCurUserDocDir() {
        return Filesystem::getCurUserHomeDir() + "/documents";
    }

    bool static dirExists(const string& dir, int mode = X_OK) {
        if (access(dir.c_str(),mode) == -1){
            return false;
        }

        return true;
    }

    string static getPathDir(const string& filePath) {
        string dirPath = filePath;
        size_t p = filePath.find_last_of('/');
        if (p != -1)
        {
            dirPath.erase(p);
        }
        return dirPath;
    }

    void static createDir(string dir, int mode = S_IRWXU) {
        if (Filesystem::dirExists(dir)){
            return;
        }

        list <string> dirList;
        string curDir = dir;
        while (true) {
            if (Filesystem::dirExists(curDir)){
                break;
            }

            dirList.push_front(curDir);

            dir = curDir;
            curDir = Filesystem::getPathDir(dir);
        }

        for (const auto& it : dirList)
        {
            int res = mkdir(it.c_str(), mode);
            std::cout<<res;
        }
    }

    vector<string> static glob(string globPattern) {
        vector<string> paths;
        glob_t tGlob;
        if(0 != ::glob(globPattern.c_str(), GLOB_ERR, NULL, &tGlob)){
            return paths;
        }

        for(int idx=0; idx<tGlob.gl_pathc; idx++){
            paths.push_back(tGlob.gl_pathv[idx]);
        }

        globfree(&tGlob);

        return paths;
    }
};

#endif //ARM_FACE_IDENTIFY_FILESYSTEM_HPP
