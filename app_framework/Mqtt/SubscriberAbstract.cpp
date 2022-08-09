//
// Created by rxwyun on 2022/7/23.
//

#include "SubscriberAbstract.h"

string SubscriberAbstract::getTopic() {
    return "";
}

SubscriberAbstract::SubscriberAbstract(Config config) : config(config) {

}

void SubscriberAbstract::setPublishClient(Client *client) {
    this->publishClient = client;
}

void SubscriberAbstract::setExceptionHandler(std::function<void(std::exception &)> exceptionHandler) {
    this->exceptionHandler = exceptionHandler;
}

Device SubscriberAbstract::getDevice() {
    return this->config.device;
}

void SubscriberAbstract::onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) {
    ;
}

HttpClient *SubscriberAbstract::getHttpClient() {
    if (this->httpClient == nullptr) {
        this->httpClient = (new HttpClient(this->config.server.httpServerAddress))->withAppId(this->config.device.appId)
                ->withAppSecret(this->config.device.appSecret);
    }

    return this->httpClient;
}

SubscriberAbstract::~SubscriberAbstract() {
    if (this->httpClient != nullptr) {
        delete this->httpClient;
        this->httpClient = nullptr;
    }
}
