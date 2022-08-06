//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_FACE_IDENTIFY_CONFIG_H
#define ARM_FACE_IDENTIFY_CONFIG_H

#include "Device.h"
#include "Mqtt.h"
#include "FaceIdentify.h"

class Config {
public:
    Device device;
    Mqtt mqtt;
    FaceIdentify faceIdentify;

    CONFIGOR_BIND(json, Config, REQUIRED(mqtt, "mqtt"), REQUIRED(device, "device"), REQUIRED(faceIdentify, "face_identify"));
};

#endif //ARM_FACE_IDENTIFY_CONFIG_H
