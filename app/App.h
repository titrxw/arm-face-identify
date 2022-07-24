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

    Client* makeMqttClient(string mqttServerAddress, string appid, string appSecret);
    Client* getMqttClient();
    SubscribeManager* getSubscribeManager();
    void startMqtt();
    void start();

protected:
    string appName = "smart_home";
    string appId = "";
    string appSecret = "";
    string mqttServerAddress = "";
    Client *client;
    SubscribeManager *subscribeManager;
};


#endif //ARM_FACE_IDENTIFY_APP_H
