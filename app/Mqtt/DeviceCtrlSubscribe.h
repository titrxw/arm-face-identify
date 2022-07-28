//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H
#define ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H

#include "./Base/SubscriberAbstract.h"

class DeviceCtrlSubscribe : virtual public SubscriberAbstract{
protected:
public:
    explicit DeviceCtrlSubscribe(Device device);
    ~DeviceCtrlSubscribe();

    string getTopic() override;
    void onSubscribe(async_client *client, const_message_ptr msg) override;
};


#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_H
