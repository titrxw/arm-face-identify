//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H
#define ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H

#include "Client.h"
#include "../../config/Device.h"
#include "../Util/CloudEvent.hpp"

class SubscriberAbstract {
public:
    explicit SubscriberAbstract(Device device, std::function<void (std::exception &e)> exceptionHandler = nullptr);
    Device getDevice();

public:
    virtual string getTopic();
    virtual void onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent);

protected:
    Device device;
    std::function<void (std::exception &e)> exceptionHandler;
};


#endif //ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H
