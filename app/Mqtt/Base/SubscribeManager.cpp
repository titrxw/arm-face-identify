//
// Created by rxwyun on 2022/7/23.
//

#include "SubscribeManager.h"
#include "Helper.h"

SubscribeManager::SubscribeManager() = default;

google_function::CloudEvent SubscribeManager::getCloudEventFromMsg(const_message_ptr msg) {
    return CloudEvent::jsonStrToCloudEvent(msg->get_payload_str());
}

void SubscribeManager::registerSubscriber(SubscriberAbstract *subscribe) {
    this->subscriberMap[subscribe->getTopic()] = subscribe;
}

void SubscribeManager::setExceptionHandler(std::function<void (async_client *client, const_message_ptr msg, std::exception e)> exceptionHandler) {
    this->exceptionHandler = exceptionHandler;
}

void SubscribeManager::start(Client *client) {
    client->getClientCallback()->onConnected = std::bind(&SubscribeManager::onConnected, this, std::placeholders::_1, std::placeholders::_2);
    client->getClientCallback()->onMessage = std::bind(&SubscribeManager::onMessage, this, std::placeholders::_1, std::placeholders::_2);

    client->connect();
}

void SubscribeManager::onConnected(async_client *client, const string &cause) {
    map<string, SubscriberAbstract*>::iterator iter;
    iter = this->subscriberMap.begin();
    while(iter != this->subscriberMap.end()) {
        client->subscribe(iter->first, 2);
        iter++;
    }
}

void SubscribeManager::onMessage(async_client *client, const_message_ptr msg) {
    std::function<void (async_client *, Device, const_message_ptr, google_function::CloudEvent, std::exception, string)> _exceptionHandler = [this](async_client *client, Device device, const_message_ptr msg, google_function::CloudEvent cloudEvent, std::exception e, string type) {
        try {
            nlohmann::json err;
            err["error"] = e.what();
            err["error_code"] = 500;
            err["payload"] = cloudEvent.data();
            cloudEvent.set_type(type + "_exception");
            cloudEvent.set_data(to_string(err));
            client->publish(msg->create(Helper::getDeviceReportTopic(device.appServerNamespace, device.appId), CloudEvent::cloudEventToJsonStr(cloudEvent)));
        } catch (std::exception reportE) {
            if (this->exceptionHandler != nullptr) {
                this->exceptionHandler(client, msg, reportE);
            }
        }

        if (this->exceptionHandler != nullptr) {
            this->exceptionHandler(client, msg, e);
        }
    };

    google_function::CloudEvent cloudEvent;
    try {
        cloudEvent = this->getCloudEventFromMsg(msg);
    } catch (nlohmann::json::exception e) {
        if (this->exceptionHandler != nullptr) {
            this->exceptionHandler(client, msg, e);
        }

        return;
    }

    map<string, SubscriberAbstract*>::iterator iter;
    iter = this->subscriberMap.find(msg->get_topic());
    if(iter != this->subscriberMap.end()){
        try {
            cloudEvent = iter->second->onSubscribe(client, msg, cloudEvent);
        } catch (std::exception e) {
            _exceptionHandler(client, iter->second->getDevice(), msg, cloudEvent, e, "ctrl");
            return;
        }

        //reply
        try {
            client->publish(msg->create(Helper::getDeviceReplayTopic(iter->second->getDevice().appServerNamespace, iter->second->getDevice().appId), CloudEvent::cloudEventToJsonStr(cloudEvent)));
        } catch (std::exception e) {
            _exceptionHandler(client, iter->second->getDevice(), msg, cloudEvent, e, "reply");
        }
    }
}

SubscribeManager::~SubscribeManager() {
    map<string, SubscriberAbstract*>::iterator iter;
    iter = this->subscriberMap.begin();
    while(iter != this->subscriberMap.end()) {
        delete iter->second;
        iter++;
    }
}
