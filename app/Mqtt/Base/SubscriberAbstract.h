//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H
#define ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H

#include <sstream>
#include "mqtt/include/mqtt/async_client.h"
#include "../../../config/Device.h"

using namespace std;
using namespace mqtt;

class SubscriberAbstract {
public:
    explicit SubscriberAbstract(Device device);

public:
    virtual string getTopic();
    virtual void onSubscribe(async_client *client, const_message_ptr msg);

protected:
    Device device;
};


#endif //ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H
