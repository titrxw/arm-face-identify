//
// Created by rxwyun on 2022/7/23.
//

#include "SubscriberAbstract.h"

string SubscriberAbstract::getTopic() {
    return "";
}

SubscriberAbstract::SubscriberAbstract(Device device) : device(device) {

}

void SubscriberAbstract::setPublishClient(Client *client) {
    this->publishClient = client;
}

void SubscriberAbstract::setExceptionHandler(std::function<void(std::exception &)> exceptionHandler) {
    this->exceptionHandler = exceptionHandler;
}

Device SubscriberAbstract::getDevice() {
    return this->device;
}

void SubscriberAbstract::onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) {
    ;
}
