//
// Created by rxwyun on 2021/9/29.
//

#include "Plugin.h"
#include <dlfcn.h>


Plugin::Plugin(string name) : name(name) {

}

PLUGIN_FUNC Plugin::getPluginFunction(const char *name) {
    auto pluginInstall = (PLUGIN_FUNC)dlsym(this->handler, name);
    if (pluginInstall == NULL) {
        throw dlerror();
    }

    return pluginInstall;
}

void Plugin::load() {
    if (this->handler) {
        return;
    }

    std::string pluginName = this->name;
    pluginName += ".so";
    this->handler = dlopen(pluginName.c_str(), RTLD_LAZY);
    if (this->handler == NULL){
        throw dlerror();
    }
}

void Plugin::install() {
    if (this->isInstall) {
        return;
    }

    this->getPluginFunction("install")();
    this->isInstall = true;
}

void Plugin::unInstall() {
    if (!this->isInstall) {
        return;
    }

    this->getPluginFunction("unInstall")();
    this->isInstall = false;
}

void Plugin::unLoad() {
    if (this->handler) {
        this->unInstall();

        dlclose(this->handler);
        this->handler = nullptr;
    }
}

Plugin::~Plugin() {
    this->unLoad();
}

