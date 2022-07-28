//
// Created by rxw on 2022/7/28.
//

#ifndef ARM_FACE_IDENTIFY_CLOUDEVENT_H
#define ARM_FACE_IDENTIFY_CLOUDEVENT_H

#include <nlohmann/json.hpp>
#include "cloudevent/cloud_event.h"

using namespace nlohmann;

class CloudEvent {
    public:
        string static cloudEventToJsonStr(google_function::CloudEvent cloudEvent) {
            json jsonObj;
            jsonObj["id"] = cloudEvent.id();
            jsonObj["source"] = cloudEvent.source();
            jsonObj["type"] = cloudEvent.type();
            jsonObj["specversion"] = cloudEvent.spec_version();
            jsonObj["datacontenttype"] = cloudEvent.data_content_type();
            jsonObj["dataschema"] = cloudEvent.data_schema();
            jsonObj["subject"] = cloudEvent.subject();
            jsonObj["time"] = cloudEvent.time();
            jsonObj["data"] = cloudEvent.data();

            return to_string(jsonObj);
        }

        google_function::CloudEvent static jsonStrToCloudEvent(const string& jsonStr) {
            json jsonObj = json::parse(jsonStr);
            if (!jsonObj.contains("id") || !jsonObj.contains("source") ||
            !jsonObj.contains("type")) {
                throw std::runtime_error(
                        "JSON message missing `id`, `source`, and/or `type` fields");
            }

            google_function::CloudEvent event = google_function::CloudEvent(
                    jsonObj.at("id").get<std::string>(), jsonObj.at("source").get<std::string>(),
                    jsonObj.at("type").get<std::string>(),
                    jsonObj.value("specversion", google_function::CloudEvent::kDefaultSpecVersion));
            if (jsonObj.count("datacontenttype") != 0) {
                event.set_data_content_type(jsonObj.at("datacontenttype").get<std::string>());
            }
            if (jsonObj.count("dataschema") != 0) {
                event.set_data_schema(jsonObj.at("dataschema").get<std::string>());
            }
            if (jsonObj.count("subject") != 0) {
                event.set_subject(jsonObj.at("subject").get<std::string>());
            }
            if (jsonObj.count("time") != 0) {
                event.set_time(jsonObj.at("time").get<std::string>());
            }
            if (jsonObj.count("data") != 0) {
                auto d = jsonObj.at("data");
                if (d.is_object()) {
                    event.set_data(d.dump());
                } else {
                    event.set_data(d.get<std::string>());
                }
            }

            return event;
        }
};

#endif //ARM_FACE_IDENTIFY_CLOUDEVENT_H
