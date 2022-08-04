//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H
#define ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H

#include "../../app_framework/Mqtt/SubscriberAbstract.h"

class DeviceCtrlSubscribe : virtual public SubscriberAbstract{
protected:
public:
    explicit DeviceCtrlSubscribe(Device device);
    ~DeviceCtrlSubscribe();

    string getTopic() override;
    google_function::CloudEvent onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) override;
};


#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H
