//
// Created by rxw on 22-12-27.
//

#ifndef ARM_IOT_IOTMESSAGE_H
#define ARM_IOT_IOTMESSAGE_H

#include "string"
#include "map"
#include "vector"

using namespace std;

namespace IOT {
    namespace MESSAGE {
        class IotMessage {
        public:
            string id;
            string eventType;
            string payload;
            string timestamp;
        };
    }
}


#endif //ARM_IOT_IOTMESSAGE_H
