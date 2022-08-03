//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H
#define ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H

#include <sstream>
#include "mqtt/include/mqtt/async_client.h"
#include "../../../config/Device.h"
#include "../../Util/CloudEvent.hpp"

using namespace std;
using namespace mqtt;

class SubscriberAbstract {
public:
    explicit SubscriberAbstract(Device device);
    Device getDevice();

public:
    virtual string getTopic();
    virtual google_function::CloudEvent onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent);

protected:
    Device device;
};


#endif //ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H
