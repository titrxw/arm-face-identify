//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_IOT_SUBSCRIBEMANAGER_H
#define ARM_IOT_SUBSCRIBEMANAGER_H

#include "map"
#include "ClientAbstract.h"
#include "SubscriberAbstract.hpp"

using namespace std;

namespace IOT {
    namespace CLIENT {
        class SubscribeManager {
        public:
            SubscribeManager(ClientAbstract *subscribeClient,
                             std::function<void(std::exception &e)> exceptionHandler = nullptr) : subscribeClient(
                    subscribeClient), exceptionHandler(exceptionHandler) {}

            ~SubscribeManager() {}

            void registerSubscriber(SubscriberAbstract *subscribe) {
                subscribe->setExceptionHandler(this->exceptionHandler);
                this->subscriberMap[subscribe->getTopic()] = subscribe;
            }

            void start() {
                this->subscribeClient->setOnConnected([this](ClientAbstract *client, const string& cause) {
                    map<string, SubscriberAbstract*>::iterator iter;
                    iter = this->subscriberMap.begin();
                    while(iter != this->subscriberMap.end()) {
                        client->subscribe(iter->first);
                        iter++;
                    }
                });
                this->subscribeClient->setOnSubscribe([this](ClientAbstract *client, string topic, string data) {
                    map<string, SubscriberAbstract*>::iterator iter;
                    iter = this->subscriberMap.find(topic);
                    if(iter != this->subscriberMap.end()) {
                        try {
                            iter->second->onSubscribe(client, IOT::UTIL::CloudEvent::jsonStrToCloudEvent(data));
                        } catch (std::exception &e) {
                            if (this->exceptionHandler != nullptr) {
                                this->exceptionHandler(e);
                            }

                            return;
                        }
                    }
                });
                this->subscribeClient->connect();
            }

        protected:
            ClientAbstract *subscribeClient = nullptr;
            map<string, SubscriberAbstract *> subscriberMap;
            std::function<void(std::exception &e)> exceptionHandler;
        };
    }
}

#endif //ARM_IOT_SUBSCRIBEMANAGER_H
