//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_APP_H
#define ARM_FACE_IDENTIFY_APP_H

#include <sstream>
#include "../config/Config.h"
#include "./Mqtt/Base/Client.h"
#include "./Mqtt/Base/SubscribeManager.h"

using namespace std;

class App {
public:
    explicit App(Config config);
    ~App();

    Client* makeMqttClient(const string& channel, Mqtt mqtt, Device device);
    Client* getDefaultMqttClient();
    SubscribeManager* getSubscribeManager();
    void start();
protected:
    void startMqtt();
protected:
    Config config;
    map<string, Client*>clientMap;
    SubscribeManager *subscribeManager{};
};


#endif //ARM_FACE_IDENTIFY_APP_H
