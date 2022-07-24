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
    string static getDeviceCtrlTopic(string topicNamespace, string appid) {
        return "/iot/" + topicNamespace + "/device/" + appid + "/ctrl";
    }

    string static getDeviceReplayTopic(string topicNamespace, string appid) {
        return "/iot/" + topicNamespace + "/device/" + appid + "/reply";
    }

    string static getDeviceReportTopic(string topicNamespace, string appid) {
        return "/iot/" + topicNamespace + "/device/" + appid + "/report";
    }
};


#endif //ARM_FACE_IDENTIFY_MQTT_H
