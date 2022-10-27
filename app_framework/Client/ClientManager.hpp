//
// Created by rxwyun on 2022/10/27.
//

#ifndef ARM_IOT_CLIENTMANAGER_H
#define ARM_IOT_CLIENTMANAGER_H

#include "map"
#include "./ClientAbstract.h"

namespace IOT {
    namespace CLIENT {
        class ClientManager {
        public:
            void registerClientResolver(const string& channel, std::function<ClientAbstract *()> resolver) {
                this->clientResolverMap[channel] = resolver;
            }

            ClientAbstract* getClient(const string& channel) {
                if (this->clientMap.end() == this->clientMap.find(channel)) {
                    this->clientMap[channel] = this->clientResolverMap[channel]();
                }

                return this->clientMap[channel];
            }

            ~ClientManager() {
                map<string, ClientAbstract*>::iterator iter;
                iter = this->clientMap.begin();
                while(iter != this->clientMap.end()) {
                    delete iter->second;
                    iter++;
                }
            }

        protected:
            map<string, std::function<ClientAbstract *()>> clientResolverMap;
            map<string, ClientAbstract*> clientMap;
        };
    }
}
#endif //ARM_IOT_CLIENTMANAGER_H
