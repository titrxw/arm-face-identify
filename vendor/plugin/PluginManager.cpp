//
// Created by rxwyun on 2021/9/29.
//

#include "PluginManager.h"


PluginManager::PluginManager() {

}

PluginManager::PluginManager(string *plugins) {
    int len= sizeof(plugins) / sizeof(string);
    for (int i = 0; i < len; i++) {
        this->load(*plugins);
        ++plugins;
    }
}

Plugin* PluginManager::load(string name) {
    Plugin *plugin = this->get(name);
    if (!plugin) {
        plugin = new Plugin(name);
        plugin->load();

        this->pluginMap[name] = plugin;
    }

    return plugin;
}

Plugin* PluginManager::get(string name) {
    if (this->pluginMap.find(name) == this->pluginMap.end()) {
        return nullptr;
    }

    return this->pluginMap[name];
}

map<string, Plugin*> PluginManager::getAll() {
    return this->pluginMap;
}

void PluginManager::unLoad(string name) {
    Plugin *plugin = this->get(name);
    if (plugin) {
        plugin->unLoad();
        this->pluginMap.erase(name);
        delete plugin;
        plugin = nullptr;
    }
}

void PluginManager::installAll() {
    for (auto it= this->pluginMap.begin(); it != this->pluginMap.end(); it++) {
        it->second->install();
    }
}

void PluginManager::unInstallAll() {
    for (auto it= this->pluginMap.begin(); it != this->pluginMap.end(); it++) {
        it->second->unInstall();
    }
}

PluginManager::~PluginManager() {
    for (auto it= this->pluginMap.begin(); it != this->pluginMap.end(); it++) {
        this->unLoad(it->first);
    }
    this->pluginMap.clear();
}

