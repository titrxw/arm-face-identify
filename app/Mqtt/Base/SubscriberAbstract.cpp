//
// Created by rxwyun on 2022/7/23.
//

#include "SubscriberAbstract.h"

string SubscriberAbstract::getTopic() {
    return "";
}

SubscriberAbstract::SubscriberAbstract(Device device) : device(device) {

}

Device SubscriberAbstract::getDevice() {
    return this->device;
}

google_function::CloudEvent SubscriberAbstract::onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) {
    return cloudEvent;
}
