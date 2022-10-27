//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_IOT_MQTT_CLIENT_H
#define ARM_IOT_MQTT_CLIENT_H

#include <sstream>
#include "mqtt/include/mqtt/async_client.h"
#include "ClientCallback.hpp"
#include "../ClientAbstract.h"

using namespace std;
using namespace mqtt;

namespace IOT {
    namespace CLIENT {
        namespace MQTT {
            class Client : virtual public IOT::CLIENT::ClientAbstract {
            public:
                Client(const string& serverAddress, const string& userName, const string& password);
                ~Client();

                async_client * getClient();
                connect_options getClientConnectOptions();
                ClientCallback* getClientCallback();
                bool connect() override;
                bool subscribe(string topic) override;
                bool publishMsg(string topic, string content) override;

            protected:
                string serverAddress;
                string userName;
                string password;
                connect_options connectOptions;
                async_client *client = nullptr;
                ClientCallback *clientCallback = nullptr;
            };
        }
    }
}

#endif //ARM_IOT_MQTT_CLIENT_H
