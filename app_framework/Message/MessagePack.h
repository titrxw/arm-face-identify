//
// Created by rxw on 22-12-27.
//

#ifndef ARM_IOT_MESSAGEPACK_H
#define ARM_IOT_MESSAGEPACK_H

#include "string"
#include "map"
#include "./IotMessage.h"
#include <nlohmann/json.hpp>

using namespace std;

namespace IOT {
    namespace MESSAGE {
        class MessagePack {
        public:
            static string pack(IotMessage message) {
                nlohmann::json jsonObj;
                jsonObj["id"] = message.id;
                jsonObj["event_type"] = message.eventType;
                jsonObj["payload"] = message.payload;
                jsonObj["timestamp"] = message.timestamp;

                return to_string(jsonObj);
            }

            static IotMessage unpack(string payload) {
                nlohmann::json jsonObj = nlohmann::json::parse(payload);
                if (!jsonObj.contains("payload") ||
                    !jsonObj.contains("event_type")) {
                    throw nlohmann::json::other_error::create(400,
                                                              "JSON message missing `event_type`, `payload`   fields",
                                                              nullptr);
                }

                IotMessage message;
                message.eventType = jsonObj.at("event_type").get<std::string>();
                message.payload = jsonObj.at("payload").get<string>();
                if (jsonObj.count("timestamp") != 0) {
                    message.timestamp = jsonObj.at("timestamp").get<std::string>();
                }
                if (jsonObj.count("id") != 0) {
                    message.id = jsonObj.at("id").get<std::string>();
                }

                return message;
            }
        };
    }
}

#endif //ARM_IOT_MESSAGEPACK_H
