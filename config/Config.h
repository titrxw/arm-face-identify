//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_FACE_IDENTIFY_CONFIG_H
#define ARM_FACE_IDENTIFY_CONFIG_H

#include "Device.h"
#include "Mqtt.h"
#include "FaceIdentify.h"
#include "Server.h"

#define APP_NAME "arm_face_identify"

class Config {
public:
    Device device;
    Mqtt mqtt;
    FaceIdentify faceIdentify;
    Server server;

    CONFIGOR_BIND(json, Config, REQUIRED(mqtt, "mqtt"), REQUIRED(server, "server"), REQUIRED(device, "device"), REQUIRED(faceIdentify, "face_identify"));
};

#endif //ARM_FACE_IDENTIFY_CONFIG_H
