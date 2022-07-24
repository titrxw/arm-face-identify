//
// Created by rxwyun on 2021/9/29.
//

#ifndef ARM_FACE_IDENTIFY_PLUGINMANAGER_H
#define ARM_FACE_IDENTIFY_PLUGINMANAGER_H

#include "map"
#include "Plugin.h"

class PluginManager {
public:
    PluginManager();
    PluginManager(string *plugins);
    ~PluginManager();

    Plugin* load(string name);
    Plugin* get(string name);
    map<string, Plugin*> getAll();
    void unLoad(string name);

    void installAll();
    void unInstallAll();

protected:
    map<string, Plugin*> pluginMap;
};

#endif //ARM_FACE_IDENTIFY_PLUGINMANAGER_H
