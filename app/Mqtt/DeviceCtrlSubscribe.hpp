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
    explicit DeviceCtrlSubscribe(Device device, Identify *identify, Train *train, std::function<void (std::exception &e)> exceptionHandler = nullptr) : SubscriberAbstract(device, exceptionHandler), identify(identify), train(train) {
        this->identify->setPredictMatMapCallback(std::bind(&DeviceCtrlSubscribe::predictedMatCallback, this, std::placeholders::_1, std::placeholders::_2));
    }
    ~DeviceCtrlSubscribe() = default;

    void predictedMatCallback(ArmFaceIdentify::PredictMat predictMat, string flag) {
        async_client *client = this->clientMap[flag];
        google_function::CloudEvent cloudEvent = this->cloudEventMap[flag];
        this->cloudEventMap.erase(flag);
        this->clientMap.erase(flag);

        Helper::publishReplyMsg(client, this->getDevice(), cloudEvent, this->exceptionHandler);
    }

    string getTopic() override {
        return Helper::getDeviceCtrlTopic(this->device.appServerNamespace, this->device.appId);
    }

    void onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) override {
        if (cloudEvent.type() == APP_OPERATE_IDENTIFY) {
            this->cloudEventMap[cloudEvent.id()] = cloudEvent;
            this->clientMap[cloudEvent.id()] = client;
            this->identify->getFaceIdentifyHandler()->setCanIdentifyNextMatWithFlag(cloudEvent.id());
        }
        if (cloudEvent.type() == APP_OPERATE_TRAIN) {

        }
    }

protected:
    Identify *identify;
    Train *train;
    map<string, google_function::CloudEvent> cloudEventMap;
    map<string, async_client *> clientMap;
};

#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
