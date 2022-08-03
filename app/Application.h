//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_APPLICATION_H
#define ARM_FACE_IDENTIFY_APPLICATION_H

#include <sstream>
#include "../config/Config.h"
#include "./Mqtt/Base/Client.h"
#include "./Mqtt/Base/SubscribeManager.h"
#include "./Exception/ExceptionHandler.hpp"
#include "spdlog/spdlog.h"

using namespace std;

class Application {
public:
    explicit Application(Config config);
    ~Application();

    Client* makeMqttClient(const string& channel, Mqtt mqtt, Device device);
    Client* getDefaultMqttClient();
    SubscribeManager* getSubscribeManager();
    void start();
protected:
    ExceptionHandler* getExceptionHandler();
    shared_ptr<spdlog::logger> getLogger();
    string getAppPath();
    string getRuntimePath();

    void initAppEnv();
    void startMqtt();
protected:
    Config config;
    map<string, Client*>clientMap;
    SubscribeManager *subscribeManager{};
    shared_ptr<spdlog::logger> logger;
    ExceptionHandler *exceptionHandler;
};


#endif //ARM_FACE_IDENTIFY_APPLICATION_H
