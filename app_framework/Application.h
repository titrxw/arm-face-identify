//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_APPLICATION_H
#define ARM_FACE_IDENTIFY_APPLICATION_H

#include <sstream>
#include "../config/Config.h"
#include "Mqtt/Client.h"
#include "Mqtt/SubscribeManager.h"
#include "./Exception/ExceptionHandler.hpp"
#include "spdlog/spdlog.h"

using namespace std;

class Application {
public:
    Application(Config config);
    ~Application();

    Client* makeMqttClient(const string& channel, Mqtt mqtt, Device device);
    Client* getPublishMqttClient();
    Client* getSubscribeMqttClient();
    SubscribeManager* getSubscribeManager();

    void start();

protected:
    ExceptionHandler* getExceptionHandler();
    shared_ptr<spdlog::logger> getLogger();
    string getAppPath();
    string getRuntimePath();

    void initAppEnv();
    void startMqtt();

    virtual void beforeStart();
    virtual void afterStart();

protected:
    Config config;
    shared_ptr<spdlog::logger> logger;
    ExceptionHandler *exceptionHandler = nullptr;
    map<string, Client*>clientMap;
    SubscribeManager *subscribeManager = nullptr;
};


#endif //ARM_FACE_IDENTIFY_APPLICATION_H
