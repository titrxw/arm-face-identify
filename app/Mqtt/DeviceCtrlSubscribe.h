//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H
#define ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H

#include "./Base/SubscriberAbstract.h"

class DeviceCtrlSubscribe : public SubscriberAbstract{
public:
    DeviceCtrlSubscribe() {}
    ~DeviceCtrlSubscribe() {}

    string getTopic() override {

    }

    void onSubscribe() override {

    }
};


#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H
