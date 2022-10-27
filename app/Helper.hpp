//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_MQTT_HELPER
#define ARM_FACE_IDENTIFY_MQTT_HELPER

#include <sstream>
#include <time.h>
#include <utility>
#include "mqtt/include/mqtt/message.h"
#include "../app_framework/Util/Encrypt.hpp"
#include "../app_framework/Util/CloudEvent.hpp"
#include "nlohmann/json.hpp"
#include "../app_framework/Client/ClientAbstract.h"

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

    void static exceptionReport(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, google_function::CloudEvent cloudEvent, std::exception &e, string type, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        try {
            nlohmann::json err;
            err["error"] = e.what();
            err["error_code"] = 500;
            err["payload"] = cloudEvent.data();
            cloudEvent.set_type(type + "_exception");
            cloudEvent.set_data(to_string(err));
            client->publishMsg(Helper::getDeviceReportTopic(device.appServerNamespace, device.appId), Helper::getMsgFromCloudEvent(cloudEvent));
        } catch (std::exception &reportE) {
            if (exceptionHandler != nullptr) {
                exceptionHandler(reportE);
            }
        }

        if (exceptionHandler != nullptr) {
            exceptionHandler(e);
        }
    }

    void static publishReportMsg(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, google_function::CloudEvent cloudEvent, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        Helper::publishMsg(client, device, Helper::getDeviceReportTopic(device.appServerNamespace, device.appId), cloudEvent, exceptionHandler);
    }

    void static publishReplySuccessMsg(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, google_function::CloudEvent cloudEvent, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        nlohmann::json payload;
        payload["status"] = "success";
        cloudEvent.set_data(to_string(payload));

        Helper::publishReplyMsg(client, device, cloudEvent, exceptionHandler);
    }

    void static publishReplyMsg(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, google_function::CloudEvent cloudEvent, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        Helper::publishMsg(client, device, Helper::getDeviceReplayTopic(device.appServerNamespace, device.appId), cloudEvent, exceptionHandler);
    }

    void static publishMsg(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, string topic, google_function::CloudEvent cloudEvent, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        try {
            client->publishMsg(topic, Helper::getMsgFromCloudEvent(cloudEvent));
        } catch (std::exception &reportE) {
            if (exceptionHandler != nullptr) {
                exceptionHandler(reportE);
            }
        }
    }

    string static getMsgFromCloudEvent(google_function::CloudEvent cloudEvent) {
        time_t timep;
        time (&timep);
        char tmp[64];
        strftime(tmp, sizeof(tmp), "%FT%TZ",localtime(&timep));
        cloudEvent.set_time(tmp);

        return IOT::UTIL::CloudEvent::cloudEventToJsonStr(cloudEvent);
    }
};


#endif //ARM_FACE_IDENTIFY_MQTT_HELPER
