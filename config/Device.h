//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_FACE_IDENTIFY_DEVICE_H
#define ARM_FACE_IDENTIFY_DEVICE_H

#include <sstream>
#include "configor/configor.hpp"
#include "configor/json.hpp"

using namespace std;
using namespace configor;

class Device {
public:
    string appName;
    string appServerNamespace;
    string appId;
    string appSecret;

    CONFIGOR_BIND(json, Device, REQUIRED(appServerNamespace, "app_server_namespace"), REQUIRED(appId, "app_id"), REQUIRED(appSecret, "app_secret"));
};

#endif //ARM_FACE_IDENTIFY_DEVICE_H
