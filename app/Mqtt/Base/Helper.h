//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_MQTT_H
#define ARM_FACE_IDENTIFY_MQTT_H

#include <sstream>

using namespace std;

const string MQTT_APP_NAME = "smart_home";

class Helper {
public:
    string static getDeviceCtrlTopic(string serverNamespace, string appid) {
        return "/iot/" + serverNamespace + "/device/" + appid + "/ctrl";
    }

    string static getDeviceReplayTopic(string serverNamespace, string appid) {
        return "/iot/" + serverNamespace + "/device/" + appid + "/reply";
    }

    string static getDeviceReportTopic(string serverNamespace, string appid) {
        return "/iot/" + serverNamespace + "/device/" + appid + "/report";
    }
};


#endif //ARM_FACE_IDENTIFY_MQTT_H
