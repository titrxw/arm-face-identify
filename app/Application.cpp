//
// Created by rxwyun on 2022/7/23.
//

#include "Application.h"
#include "./Mqtt/DeviceCtrlSubscribe.h"
#include "./Util/Filesystem.hpp"
#include "spdlog/sinks/daily_file_sink.h"

Application::Application(Config config) : config(config) {

}

string Application::getAppPath() {
    return Filesystem::getCurUserDocDir() + "/" + this->config.app.appName;
}

string Application::getRuntimePath() {
    return this->getAppPath() + "/runtimes";
}

shared_ptr<spdlog::logger> Application::getLogger() {
    if (this->logger != nullptr) {
        return this->logger;
    }

    string logPath = this->getRuntimePath() + "/log";
    if (!Filesystem::dirExists(logPath)) {
        Filesystem::createDir(logPath);
    }

    this->logger = spdlog::daily_logger_mt("daily_logger", logPath + "/daily.txt", 0, 0);
    this->logger->set_level(spdlog::level::debug);
    this->logger->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
    spdlog::set_default_logger(this->logger);

    return this->logger;
}

ExceptionHandler *Application::getExceptionHandler() {
    if (this->exceptionHandler == nullptr) {
        this->exceptionHandler = new ExceptionHandler(this->getLogger());
    }

    return this->exceptionHandler;
}

Client *Application::makeMqttClient(const string& channel, Mqtt mqtt, Device device) {
    if (this->clientMap.end() == this->clientMap.find(channel)) {
        this->clientMap[channel] = new Client(mqtt.mqttServerAddress, device.appId, device.appSecret);
    }

    return this->clientMap[channel];
}

Client *Application::getDefaultMqttClient() {
    return this->makeMqttClient("default", this->config.mqtt, this->config.device);
}

SubscribeManager *Application::getSubscribeManager() {
    if (this->subscribeManager == nullptr) {
        this->subscribeManager = new SubscribeManager();
    }

    return this->subscribeManager;
}

void Application::startMqtt() {
    subscribeManager = this->getSubscribeManager();
    subscribeManager->registerSubscriber(new DeviceCtrlSubscribe(this->config.device));
    subscribeManager->setExceptionHandler([this](async_client *client, const_message_ptr msg, std::exception e) {
        this->getExceptionHandler()->handle(e);
    });
    subscribeManager->start(this->getDefaultMqttClient());
}

void Application::initAppEnv() {
    if (!Filesystem::dirExists(this->getRuntimePath())) {
        Filesystem::createDir(this->getRuntimePath());
    }
}

void Application::start() {
    try {
        this->initAppEnv();
        this->startMqtt();
    } catch (std::exception &e) {
        this->getExceptionHandler()->handle(e);
        throw e;
    }
}

Application::~Application() {
    delete this->subscribeManager;
    delete this->exceptionHandler;
    spdlog::drop_all();

    map<string, Client*>::iterator iter;
    iter = this->clientMap.begin();
    while(iter != this->clientMap.end()) {
        delete iter->second;
        iter++;
    }
}
