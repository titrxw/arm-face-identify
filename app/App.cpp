//
// Created by rxwyun on 2022/7/23.
//

#include "App.h"
#include "./Mqtt/DeviceCtrlSubscribe.h"
#include "./Util/Filesystem.h"
#include "spdlog/sinks/daily_file_sink.h"

App::App(Config config) : config(config) {

}

string App::getAppPath() {
    return Filesystem::getCurUserDocDir();
}

string App::getRuntimePath() {
    return this->getAppPath() + "/runtimes";
}

shared_ptr<spdlog::logger> App::getLogger() {
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

ExceptionHandler *App::getExceptionHandler() {
    if (this->exceptionHandler == nullptr) {
        this->exceptionHandler = new ExceptionHandler(this->getLogger());
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

void App::initAppEnv() {
    if (!Filesystem::dirExists(this->getRuntimePath())) {
        Filesystem::createDir(this->getRuntimePath());
    }
}

void App::start() {
    this->initAppEnv();
    this->startMqtt();
}

App::~App() {
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
