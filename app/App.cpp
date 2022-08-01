//
// Created by rxwyun on 2022/7/23.
//

#include "App.h"

#include "./Mqtt/DeviceCtrlSubscribe.h"

App::App(Config config) : config(config) {

}

ExceptionHandler *App::getExceptionHandler() {
    if (this->exceptionHandler == nullptr) {
        this->exceptionHandler = new ExceptionHandler();
    }

    return this->exceptionHandler;
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
    subscribeManager->setExceptionHandler([this](async_client *client, const_message_ptr msg, std::exception e) {
        this->getExceptionHandler()->report(e);
        this->getExceptionHandler()->handle(e);
    });
}

void App::start() {
    this->startMqtt();
}

App::~App() {
    delete this->subscribeManager;
    delete this->exceptionHandler;

    map<string, Client*>::iterator iter;
    iter = this->clientMap.begin();
    while(iter != this->clientMap.end()) {
        delete iter->second;
        iter++;
    }
}
