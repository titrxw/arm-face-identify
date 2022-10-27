//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_IOT_CONFIG_H
#define ARM_IOT_CONFIG_H

#include "Device.h"
#include "Mqtt.h"

namespace IOT {
    namespace CONFIG {
        class Config {
        public:
            Device device;
            Mqtt mqtt;

            CONFIGOR_BIND(json, Config, REQUIRED(mqtt, "mqtt"), REQUIRED(device, "device"));
        };
    }
}

#endif //ARM_IOT_CONFIG_H
