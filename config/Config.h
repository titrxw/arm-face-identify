//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_FACE_IDENTIFY_CONFIG_H
#define ARM_FACE_IDENTIFY_CONFIG_H

#include "Device.h"
#include "Mqtt.h"

class Config {
public:
    string appName;
    Device device;
    Mqtt mqtt;

    CONFIGOR_BIND(json, Config, REQUIRED(appName, "app_name"), REQUIRED(mqtt, "mqtt"), REQUIRED(device, "device"));
};

#endif //ARM_FACE_IDENTIFY_CONFIG_H
