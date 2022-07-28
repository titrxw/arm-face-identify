//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_FACE_IDENTIFY_MQTT_H
#define ARM_FACE_IDENTIFY_MQTT_H

#include <sstream>
#include "configor/configor.hpp"
#include "configor/json.hpp"

using namespace std;
using namespace configor;

class Mqtt {
public:
    string mqttServerAddress;
    CONFIGOR_BIND(json, Mqtt, REQUIRED(mqttServerAddress, "server_address"));
};

#endif //ARM_FACE_IDENTIFY_MQTT_H
