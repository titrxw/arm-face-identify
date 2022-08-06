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
#include "nlohmann/json.hpp"

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

    void static exceptionReport(async_client *client, Device device, google_function::CloudEvent cloudEvent, std::exception &e, string type, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        try {
            nlohmann::json err;
            err["error"] = e.what();
            err["error_code"] = 500;
            err["payload"] = cloudEvent.data();
            cloudEvent.set_type(type + "_exception");
            cloudEvent.set_data(to_string(err));
            client->publish(Helper::getMsgFromCloudEvent(Helper::getDeviceReportTopic(device.appServerNamespace, device.appId), cloudEvent, device.appSecret));
        } catch (std::exception reportE) {
            if (exceptionHandler != nullptr) {
                exceptionHandler(reportE);
            }
        }

        if (exceptionHandler != nullptr) {
            exceptionHandler(e);
        }
    }

    void static publishReportMsg(async_client *client, Device device, google_function::CloudEvent cloudEvent, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        Helper::publishMsg(client, device, Helper::getDeviceReportTopic(device.appServerNamespace, device.appId), cloudEvent, exceptionHandler);
    }

    void static publishReplyMsg(async_client *client, Device device, google_function::CloudEvent cloudEvent, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        Helper::publishMsg(client, device, Helper::getDeviceReplayTopic(device.appServerNamespace, device.appId), cloudEvent, exceptionHandler);
    }

    void static publishMsg(async_client *client, Device device, string topic, google_function::CloudEvent cloudEvent, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        try {
            client->publish(Helper::getMsgFromCloudEvent(topic, cloudEvent, device.appSecret));
        } catch (std::exception reportE) {
            if (exceptionHandler != nullptr) {
                exceptionHandler(reportE);
            }
        }
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
