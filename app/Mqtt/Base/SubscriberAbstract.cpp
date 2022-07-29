//
// Created by rxwyun on 2022/7/23.
//

#include "SubscriberAbstract.h"

string SubscriberAbstract::getTopic() {
    return "";
}

SubscriberAbstract::SubscriberAbstract(Device device) : device(device) {

}

google_function::CloudEvent SubscriberAbstract::getCloudEventFromMsg(const_message_ptr msg) {
    return CloudEvent::jsonStrToCloudEvent(msg->get_payload_str());
}

void SubscriberAbstract::onSubscribe(async_client *client, const_message_ptr msg) {}
