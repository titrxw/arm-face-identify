//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
#define ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP

#include "../../app_framework/Mqtt/SubscriberAbstract.h"
#include "../../app_framework/Mqtt/Helper.hpp"
#include "../Face/Identify.h"
#include "../Face/Train.h"
#include "../Define.h"

class DeviceCtrlSubscribe : virtual public SubscriberAbstract{
public:
    explicit DeviceCtrlSubscribe(Device device, Identify *identify, Train *train) : SubscriberAbstract(device), identify(identify), train(train) {
        this->identify->setPredictMatMapCallback(DeviceCtrlSubscribe::predictedMatCallback);
    }
    ~DeviceCtrlSubscribe() = default;

    void static predictedMatCallback(ArmFaceIdentify::PredictMat predictMat, string flag) {

    }

    string getTopic() override {
        return Helper::getDeviceCtrlTopic(this->device.appServerNamespace, this->device.appId);
    }

    void onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) override {
        if (cloudEvent.type() == APP_OPERATE_IDENTIFY) {
            this->identify->getFaceIdentifyHandler()->setCanIdentifyNextMatWithFlag(cloudEvent.id());
        }
        if (cloudEvent.type() == APP_OPERATE_TRAIN) {

        }
    }

protected:
    Identify *identify;
    Train *train;
};

#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
