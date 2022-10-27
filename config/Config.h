//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_FACE_IDENTIFY_CONFIG_H
#define ARM_FACE_IDENTIFY_CONFIG_H

#include "FaceIdentify.h"
#include "Server.h"
#include "../app_framework/Config/Config.h"

#define APP_NAME "arm_face_identify"

class Config : public IOT::CONFIG::Config {
public:
    FaceIdentify faceIdentify;
    Server server;

    CONFIGOR_BIND(json, Config, REQUIRED(server, "server"), REQUIRED(faceIdentify, "face_identify"));
};

#endif //ARM_FACE_IDENTIFY_CONFIG_H
