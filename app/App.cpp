//
// Created by rxwyun on 2022/7/23.
//

#include "App.h"
#include "./Mqtt/DeviceCtrlSubscribe.h"

App::App() {

}

Client *App::makeMqttClient(string mqttServerAddress, string appid, string appSecret) {
    return new Client(mqttServerAddress, appid, appSecret);
}

Client *App::getMqttClient() {
    if (this->client == nullptr) {
        this->client = this->makeMqttClient(this->mqttServerAddress, this->appId, this->appSecret);
    }

    return this->client;
}

void App::startMqtt() {
    subscribeManager = this->getSubscribeManager();
    subscribeManager->registerSubscriber(new DeviceCtrlSubscribe());
    subscribeManager->start(this->getMqttClient());
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
    if (this->client != nullptr) {
        delete this->client;
    }
}
