//
// Created by rxwyun on 2022/7/23.
//

#include "SubscribeManager.h"

SubscribeManager::SubscribeManager() {

}

void SubscribeManager::registerSubscriber(SubscriberAbstract *subscribe) {
    this->subscriberMap[subscribe->getTopic()] = subscribe;
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

}

SubscribeManager::~SubscribeManager() {
    map<string, SubscriberAbstract*>::iterator iter;
    iter = this->subscriberMap.begin();
    while(iter != this->subscriberMap.end()) {
        delete iter->second;
        iter++;
    }
}
