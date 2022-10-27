//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_IOT_MQTT_H
#define ARM_IOT_MQTT_H

#include <sstream>
#include "configor/configor.hpp"
#include "configor/json.hpp"

using namespace std;
using namespace configor;

namespace IOT {
    namespace CONFIG {
        class Mqtt {
        public:
            string mqttServerAddress;

            CONFIGOR_BIND(json, Mqtt, REQUIRED(mqttServerAddress, "server_address"));
        };
    }
}
#endif //ARM_IOT_MQTT_H
