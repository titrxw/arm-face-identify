//
// Created by rxwyun on 2022/7/23.
//

#include "App.h"
#include "./Mqtt/DeviceCtrlSubscribe.h"

App::App() {

}

Client *App::makeMqttClient(string channel, string mqttServerAddress, string appid, string appSecret) {
    if (this->clientMap.end() == this->clientMap.find(channel)) {
        this->clientMap[channel] = new Client(mqttServerAddress, appid, appSecret);
    }

    return this->clientMap[channel];
}

Client *App::getDefaultMqttClient() {
    return this->makeMqttClient("default", this->mqttServerAddress, this->appId, this->appSecret);
}

void App::startMqtt() {
    subscribeManager = this->getSubscribeManager();
    subscribeManager->registerSubscriber(new DeviceCtrlSubscribe(this->appName, this->appId));
    subscribeManager->start(this->getDefaultMqttClient());
}

SubscribeManager *App::getSubscribeManager() {
    if (this->subscribeManager == nullptr) {
        this->subscribeManager = new SubscribeManager();
    }

    return this->subscribeManager;
}

void App::start() {
    this->startMqtt();
}

App::~App() {
    if (this->subscribeManager != nullptr) {
        delete this->subscribeManager;
    }
    map<string, Client*>::iterator iter;
    iter = this->clientMap.begin();
    while(iter != this->clientMap.end()) {
        delete iter->second;
        iter++;
    }
}
