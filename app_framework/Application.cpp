//
// Created by rxwyun on 2022/7/23.
//

#include "Application.h"
#include "./Util/Filesystem.hpp"
#include "spdlog/sinks/daily_file_sink.h"

Application::Application(Config config) : config(config) {

}

string Application::getAppPath() {
    return Filesystem::getCurUserDocDir() + "/" + this->config.device.appName;
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

void Application::initAppEnv() {
    if (!Filesystem::dirExists(this->getRuntimePath())) {
        Filesystem::createDir(this->getRuntimePath());
    }
}


Client *Application::makeMqttClient(const string& channel, Mqtt mqtt, Device device) {
    if (this->clientMap.end() == this->clientMap.find(channel)) {
        this->clientMap[channel] = new Client(mqtt.mqttServerAddress, device.appId, device.appSecret);
    }

    return this->clientMap[channel];
}

Client *Application::getPublishMqttClient() {
    return this->makeMqttClient("subscribe", this->config.mqtt, this->config.device);
}

Client *Application::getSubscribeMqttClient() {
    return this->makeMqttClient("subscribe", this->config.mqtt, this->config.device);
}

SubscribeManager *Application::getSubscribeManager() {
    if (this->subscribeManager == nullptr) {
        this->subscribeManager = new SubscribeManager(this->getSubscribeMqttClient(), this->getPublishMqttClient(), [this](std::exception &e) {
            this->getExceptionHandler()->handle(e);
        });
    }

    return this->subscribeManager;
}

void Application::startMqtt() {
    subscribeManager = this->getSubscribeManager();
    subscribeManager->start();
}

void Application::beforeStart() {

}

void Application::afterStart() {

}

void Application::start() {
    try {
        this->initAppEnv();
        this->beforeStart();
        this->startMqtt();
        this->afterStart();
    } catch (std::exception &e) {
        this->getExceptionHandler()->handle(e);
        throw e;
    }
}

Application::~Application() {
    delete this->subscribeManager;
    this->subscribeManager = nullptr;

    delete this->exceptionHandler;
    this->exceptionHandler = nullptr;

    spdlog::drop_all();

    map<string, Client*>::iterator iter;
    iter = this->clientMap.begin();
    while(iter != this->clientMap.end()) {
        delete iter->second;
        iter++;
    }
}
