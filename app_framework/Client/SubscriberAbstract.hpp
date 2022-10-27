//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_IOT_SUBSCRIBERABSTRACT_H
#define ARM_IOT_SUBSCRIBERABSTRACT_H

#include "../../../config/Config.h"
#include "../Util/CloudEvent.hpp"
#include "./ClientAbstract.h"

namespace IOT {
    namespace CLIENT {
        class SubscriberAbstract {
        public:
            explicit SubscriberAbstract(CONFIG::Device device) : device(device) {}

            ~SubscriberAbstract() = default;

            string getTopic() {
                return "";
            }

            void setExceptionHandler(std::function<void(std::exception &e)> exceptionHandler) {
                this->exceptionHandler = exceptionHandler;
            }

            virtual void onSubscribe(ClientAbstract *client, google_function::CloudEvent cloudEvent) {

            }

        protected:
            CONFIG::Device device;
            std::function<void(std::exception &e)> exceptionHandler;
        };
    }
}

#endif //ARM_IOT_SUBSCRIBERABSTRACT_H
