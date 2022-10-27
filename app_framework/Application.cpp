//
// Created by rxwyun on 2022/7/23.
//

#include "Application.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "Util/Filesystem.hpp"
#include "./Client/Mqtt/Client.h"

IOT::Application::Application(CONFIG::Config *config) : config(config) {

}

string IOT::Application::getAppPath() {
    return IOT::UTIL::Filesystem::getCurUserDocDir() + "/" + this->config->device.appName;
}

string IOT::Application::getRuntimePath() {
    return this->getAppPath() + "/runtimes";
}

shared_ptr<spdlog::logger> IOT::Application::getLogger() {
    if (this->logger != nullptr) {
        return this->logger;
    }

    string logPath = this->getRuntimePath() + "/log";
    if (!IOT::UTIL::Filesystem::dirExists(logPath)) {
        IOT::UTIL::Filesystem::createDir(logPath);
    }

    this->logger = spdlog::daily_logger_mt("daily_logger", logPath + "/daily.txt", 0, 0);
    this->logger->set_level(spdlog::level::debug);
    this->logger->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
    spdlog::set_default_logger(this->logger);

    return this->logger;
}

IOT::ExceptionHandler *IOT::Application::getExceptionHandler() {
    if (this->exceptionHandler == nullptr) {
        this->exceptionHandler = new ExceptionHandler(this->getLogger());
    }

    return this->exceptionHandler;
}

void IOT::Application::initAppEnv() {
    if (!IOT::UTIL::Filesystem::dirExists(this->getRuntimePath())) {
        IOT::UTIL::Filesystem::createDir(this->getRuntimePath());
    }
}

IOT::CLIENT::ClientAbstract *IOT::Application::getDefaultClient() {
    return this->getClientManager()->getClient("default");
}

void IOT::Application::registerClient() {
    this->getClientManager()->registerClientResolver("default", [this]() -> CLIENT::ClientAbstract* {
        return new IOT::CLIENT::MQTT::Client(this->config->mqtt.mqttServerAddress, this->config->device.appId, this->config->device.appSecret);
    });
}

IOT::CLIENT::ClientManager *IOT::Application::getClientManager() {
    if (this->clientManager == nullptr) {
        this->clientManager = new CLIENT::ClientManager();
    }

    return this->clientManager;
}

IOT::CLIENT::SubscribeManager* IOT::Application::getSubscribeManager() {
    if (this->subscribeManager == nullptr) {
        this->subscribeManager = new IOT::CLIENT::SubscribeManager(this->getDefaultClient(), [this](std::exception &e) {
            this->getExceptionHandler()->handle(e);
        });
    }

    return this->subscribeManager;
}

void IOT::Application::startSubscribe() {
    this->getSubscribeManager()->start();
}

void IOT::Application::beforeStart() {

}

void IOT::Application::afterStart() {

}

void IOT::Application::start() {
    try {
        this->initAppEnv();
        this->registerClient();
        this->beforeStart();
        this->startSubscribe();
        this->afterStart();
    } catch (std::exception &e) {
        this->getExceptionHandler()->handle(e);

        string msg = "error occur, reason: ";
        std::cout<< msg.append(e.what()) << endl;

        throw e;
    }
}

IOT::Application::~Application() {
    delete this->clientManager;
    this->clientManager = nullptr;

    delete this->subscribeManager;
    this->subscribeManager = nullptr;

    delete this->exceptionHandler;
    this->exceptionHandler = nullptr;

    spdlog::drop_all();
}
