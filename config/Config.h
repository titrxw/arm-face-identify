//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_FACE_IDENTIFY_CONFIG_H
#define ARM_FACE_IDENTIFY_CONFIG_H

#include "App.h"
#include "Device.h"
#include "Mqtt.h"

class Config {
public:
    App app;
    Device device;
    Mqtt mqtt;

    CONFIGOR_BIND(json, Config, REQUIRED(app, "app"), REQUIRED(mqtt, "mqtt"), REQUIRED(device, "device"));
};

#endif //ARM_FACE_IDENTIFY_CONFIG_H
