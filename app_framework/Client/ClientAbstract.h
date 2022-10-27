//
// Created by rxwyun on 2022/10/27.
//

#ifndef ARM_IOT_MSGHANDLERABSTRACT_H
#define ARM_IOT_MSGHANDLERABSTRACT_H

#include "string"

using namespace std;

namespace IOT {
    namespace CLIENT {
        class ClientAbstract {
        public:
            ClientAbstract() {}
            ~ClientAbstract(){}

            void setOnConnected(std::function<void (ClientAbstract *client, const string& cause)> onConnected) {
                this->onConnected = onConnected;
            }

            void setOnMessage(std::function<void (ClientAbstract *client, string data)> onMessage) {
                this->onMessage = onMessage;
            }

            void setOnSubscribe(std::function<void (ClientAbstract *client, string topic, string data)> onSubscribe) {
                this->onSubscribe = onSubscribe;
            }

            virtual bool publishMsg(string topic, string content) {
                return false;
            }

            virtual bool subscribe(string topic) {
                return false;
            }

            virtual bool connect() {
                return false;
            }

        protected:
            std::function<void (ClientAbstract *client, const string& cause)> onConnected;
            std::function<void (ClientAbstract *client, string data)> onMessage;
            std::function<void (ClientAbstract *client, string topic, string data)> onSubscribe;
        };
    }
}

#endif //ARM_IOT_MSGHANDLERABSTRACT_H
