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
            iter->second->onSubscribe(client, msg, cloudEvent);
        } catch (std::exception e) {
            try {
                nlohmann::json err;
                err["error"] = e.what();
                err["error_code"] = 500;
                cloudEvent.set_type("exception");
                cloudEvent.set_data(to_string(err));
                client->publish(msg->create(Helper::getDeviceReportTopic(iter->second->getDevice().appServerNamespace, iter->second->getDevice().appId), CloudEvent::cloudEventToJsonStr(cloudEvent)));
            } catch (std::exception reportE) {
                if (this->exceptionHandler != nullptr) {
                    this->exceptionHandler(client, msg, reportE);
                }
            }

            if (this->exceptionHandler != nullptr) {
                this->exceptionHandler(client, msg, e);
            }
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
