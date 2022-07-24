//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_APP_H
#define ARM_FACE_IDENTIFY_APP_H

#include <sstream>
#include "./Mqtt/Base/Client.h"
#include "./Mqtt/Base/SubscribeManager.h"

using namespace std;

class App {
public:
    App();
    ~App();

    Client* makeMqttClient(string channel, string mqttServerAddress, string appid, string appSecret);
    Client* getDefaultMqttClient();
    SubscribeManager* getSubscribeManager();
    void start();
protected:
    void startMqtt();
protected:
    string appName = "smart_home";
    string appId = "";
    string appSecret = "";
    string mqttServerAddress = "tcp://127.0.0.1:1883";
    map<string, Client*>clientMap;
    SubscribeManager *subscribeManager;
};


#endif //ARM_FACE_IDENTIFY_APP_H
