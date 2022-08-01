//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_SUBSCRIBEMANAGER_H
#define ARM_FACE_IDENTIFY_SUBSCRIBEMANAGER_H

#include "map"
#include "SubscriberAbstract.h"
#include "Client.h"

using namespace std;

class SubscribeManager {
public:
    SubscribeManager();
    ~SubscribeManager();

    void registerSubscriber(SubscriberAbstract *subscribe);
    void setExceptionHandler(std::function<void (async_client *client, const_message_ptr msg, std::exception e)>);
    void onConnected(async_client *client, const string& cause);
    void onMessage(async_client *client, const_message_ptr msg);

    void start(Client *client);
protected:
    google_function::CloudEvent getCloudEventFromMsg(const_message_ptr msg);
protected:
    map<string, SubscriberAbstract*> subscriberMap;
    std::function<void (async_client *client, const_message_ptr msg, std::exception e)> exceptionHandler;
};


#endif //ARM_FACE_IDENTIFY_SUBSCRIBEMANAGER_H
