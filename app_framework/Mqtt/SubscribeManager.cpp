//
// Created by rxwyun on 2022/7/23.
//

#include "SubscribeManager.h"
#include "Helper.hpp"
#include "nlohmann/json.hpp"

SubscribeManager::SubscribeManager() = default;

void SubscribeManager::registerSubscriber(SubscriberAbstract *subscribe) {
    this->subscriberMap[subscribe->getTopic()] = subscribe;
}

void SubscribeManager::setExceptionHandler(std::function<void (std::exception &e)> exceptionHandler) {
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
    map<string, SubscriberAbstract*>::iterator iter;
    iter = this->subscriberMap.find(msg->get_topic());
    if(iter != this->subscriberMap.end()) {
        Device device = iter->second->getDevice();

        google_function::CloudEvent cloudEvent;
        try {
            cloudEvent = Helper::getCloudEventFromMsg(msg, device.appSecret);
        } catch (nlohmann::json::exception &e) {
            if (this->exceptionHandler != nullptr) {
                this->exceptionHandler(e);
            }

            return;
        }

        try {
            iter->second->onSubscribe(client, msg, cloudEvent);
        } catch (std::exception &e) {
            Helper::exceptionReport(client, device, cloudEvent, e, "ctrl", this->exceptionHandler);
            return;
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
