//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_MQTT_CLIENT_H
#define ARM_FACE_IDENTIFY_MQTT_CLIENT_H

#include <sstream>
#include "mqtt/include/mqtt/async_client.h"
#include "ClientCallback.hpp"

using namespace std;
using namespace mqtt;

class Client {
public:
    Client(const string& serverAddress, const string& userName, const string& password);
    ~Client();

    async_client * getClient();
    connect_options getClientConnectOptions();
    ClientCallback* getClientCallback();
    void connect();

protected:
    string serverAddress;
    string userName;
    string password;
    connect_options connectOptions;
    async_client *client = nullptr;
    ClientCallback *clientCallback = nullptr;
};


#endif //ARM_FACE_IDENTIFY_MQTT_CLIENT_H
