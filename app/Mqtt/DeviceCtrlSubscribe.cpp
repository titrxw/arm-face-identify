//
// Created by rxw on 2022/7/28.
//

#include "DeviceCtrlSubscribe.h"
#include "./Base/Helper.h"

DeviceCtrlSubscribe::DeviceCtrlSubscribe(Device device) : SubscriberAbstract(device) {

}

string DeviceCtrlSubscribe::getTopic() {
    return Helper::getDeviceCtrlTopic(this->device.appServerNamespace, this->device.appId);
}

void
DeviceCtrlSubscribe::onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) {

}

DeviceCtrlSubscribe::~DeviceCtrlSubscribe() {

}
