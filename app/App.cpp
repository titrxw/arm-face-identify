//
// Created by rxwyun on 2022/7/23.
//

#include "App.h"

#include "./Mqtt/DeviceCtrlSubscribe.h"

App::App(Config config) : config(config) {

}

Client *App::makeMqttClient(const string& channel, Mqtt mqtt, Device device) {
    if (this->clientMap.end() == this->clientMap.find(channel)) {
        this->clientMap[channel] = new Client(mqtt.mqttServerAddress, device.appId, device.appSecret);
    }

    return this->clientMap[channel];
}

Client *App::getDefaultMqttClient() {
    return this->makeMqttClient("default", this->config.mqtt, this->config.device);
}

SubscribeManager *App::getSubscribeManager() {
    if (this->subscribeManager == nullptr) {
        this->subscribeManager = new SubscribeManager();
    }

    return this->subscribeManager;
}

void App::startMqtt() {
    subscribeManager = this->getSubscribeManager();
    subscribeManager->registerSubscriber(new DeviceCtrlSubscribe(this->config.device));
    subscribeManager->start(this->getDefaultMqttClient());
}

void App::start() {
    this->startMqtt();
}

App::~App() {
    delete this->subscribeManager;

    map<string, Client*>::iterator iter;
    iter = this->clientMap.begin();
    while(iter != this->clientMap.end()) {
        delete iter->second;
        iter++;
    }
}
