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
#include "../app_framework/Message/IotMessage.h"
#include "../app_framework/Message/MessagePack.h"

using namespace std;

class Helper {
public:
    string static getDeviceCtrlTopic(const string& topicNamespace, const string& appid) {
        return "/iot/" + topicNamespace + "/device/" + appid + "/ctrl";
    }

    string static getDeviceReportTopic(const string& topicNamespace, const string& appid) {
        return "/iot/" + topicNamespace + "/device/" + appid + "/report";
    }

    void static exceptionReport(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, IOT::MESSAGE::IotMessage message, std::exception &e, string type, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        try {
            nlohmann::json err;
            err["error"] = e.what();
            err["error_code"] = 500;
            err["payload"] = message.payload;
            message.eventType = type + "_exception";
            message.payload = to_string(err);
            client->publishMsg(Helper::getDeviceReportTopic(device.appServerNamespace, device.appId), Helper::getMsgFromCloudEvent(message));
        } catch (std::exception &reportE) {
            if (exceptionHandler != nullptr) {
                exceptionHandler(reportE);
            }
        }

        if (exceptionHandler != nullptr) {
            exceptionHandler(e);
        }
    }

    void static publishReportMsg(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, IOT::MESSAGE::IotMessage message, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        Helper::publishMsg(client, device, Helper::getDeviceReportTopic(device.appServerNamespace, device.appId), message, exceptionHandler);
    }

    void static publishReplySuccessMsg(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, IOT::MESSAGE::IotMessage message, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        nlohmann::json payload;
        payload["status"] = "success";
        message.payload = to_string(payload);

        Helper::publishReplyMsg(client, device, message, exceptionHandler);
    }

    void static publishReplyMsg(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, IOT::MESSAGE::IotMessage message, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        Helper::publishMsg(client, device, Helper::getDeviceReportTopic(device.appServerNamespace, device.appId), message, exceptionHandler);
    }

    void static publishMsg(IOT::CLIENT::ClientAbstract *client, IOT::CONFIG::Device device, string topic, IOT::MESSAGE::IotMessage message, std::function<void (std::exception &e)> exceptionHandler = nullptr) {
        try {
            client->publishMsg(topic, Helper::getMsgFromCloudEvent(message));
        } catch (std::exception &reportE) {
            if (exceptionHandler != nullptr) {
                exceptionHandler(reportE);
            }
        }
    }

    string static getMsgFromCloudEvent(IOT::MESSAGE::IotMessage message) {
        message.timestamp = time(0);

        return IOT::MESSAGE::MessagePack::pack(message);
    }
};


#endif //ARM_FACE_IDENTIFY_MQTT_HELPER
