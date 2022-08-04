//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_MQTT_HELPER
#define ARM_FACE_IDENTIFY_MQTT_HELPER

#include <sstream>
#include <time.h>
#include <utility>
#include "mqtt/include/mqtt/message.h"
#include "../Util/Encrypt.hpp"
#include "../Util/CloudEvent.hpp"

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

    const_message_ptr static getMsgFromCloudEvent(const string& topic, google_function::CloudEvent cloudEvent, const string& appSecret) {
        const_message_ptr msg;
        time_t timep;
        time (&timep);
        char tmp[64];
        strftime(tmp, sizeof(tmp), "%FT%TZ",localtime(&timep));
        cloudEvent.set_time(tmp);
        return msg->create(topic, Encrypt::encrypt(CloudEvent::cloudEventToJsonStr(cloudEvent), appSecret));
    }

    google_function::CloudEvent static getCloudEventFromMsg(const const_message_ptr& msg, const string& appSecret) {
        string payload;
        try {
            payload = Encrypt::decrypt(msg->get_payload_str(), appSecret);
        } catch (std::exception e) {
            payload = "";
        }

        return CloudEvent::jsonStrToCloudEvent(payload);
    }
};


#endif //ARM_FACE_IDENTIFY_MQTT_HELPER
