//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H
#define ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H

#include "./Base/SubscriberAbstract.h"
#include "./Base/Helper.h"

class DeviceCtrlSubscribe : virtual public SubscriberAbstract{
protected:
    string topicNamespace;
    string appid;
public:
    DeviceCtrlSubscribe(string topicNamespace, string appid) : topicNamespace(topicNamespace), appid(appid) {}
    ~DeviceCtrlSubscribe() {}

    string getTopic() override {
        return Helper::getDeviceCtrlTopic(this->topicNamespace, this->appid);
    }

    void onSubscribe() override {

    }
};


#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H
