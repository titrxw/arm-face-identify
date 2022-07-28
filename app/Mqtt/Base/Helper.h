//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_MQTT_HELPER
#define ARM_FACE_IDENTIFY_MQTT_HELPER

#include <sstream>

using namespace std;

class Helper {
public:
    string static getDeviceCtrlTopic(const string& topicNamespace, const string& appid) {
        return "/iot/" + topicNamespace + "/device/" + appid + "/ctrl";
    }

    string static getDeviceReplayTopic(const string& topicNamespace, const string& appid) {
        return "/iot/" + topicNamespace + "/device/" + appid + "/reply";
    }

    string static getDeviceReportTopic(const string& topicNamespace, const string& appid) {
        return "/iot/" + topicNamespace + "/device/" + appid + "/report";
    }
};


#endif //ARM_FACE_IDENTIFY_MQTT_HELPER
