//
// Created by rxwyun on 2021/9/29.
//

#ifndef ARM_FACE_IDENTIFY_PLUGIN_H
#define ARM_FACE_IDENTIFY_PLUGIN_H

#include "string"

using namespace std;

typedef void (*PLUGIN_FUNC)();

class Plugin {
public:
    explicit Plugin(string name);
    ~Plugin();

    void load();
    PLUGIN_FUNC getPluginFunction(const char *name);
    void install();
    void unInstall();
    void unLoad();

protected:
    string name;
    void *handler = nullptr;
    bool isInstall = false;
};

#endif //ARM_FACE_IDENTIFY_PLUGIN_H
