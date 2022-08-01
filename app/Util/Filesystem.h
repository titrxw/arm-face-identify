//
// Created by rxw on 2022/8/1.
//

#ifndef ARM_FACE_IDENTIFY_FILESYSTEM_H
#define ARM_FACE_IDENTIFY_FILESYSTEM_H

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

        return path + "/documents";
    }

    string static getCurUserDocDir() {
        return Filesystem::getCurUserHomeDir() + "/documents";
    }
};

#endif //ARM_FACE_IDENTIFY_FILESYSTEM_H
