//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_IOT_APPLICATION_H
#define ARM_IOT_APPLICATION_H

#include <sstream>
#include "./Exception/ExceptionHandler.hpp"
#include "./Config/Config.h"
#include "spdlog/spdlog.h"
#include "./Client/ClientManager.hpp"
#include "Client/SubscribeManager.hpp"

using namespace std;


namespace IOT {
    class Application {
    public:
        Application(CONFIG::Config *config);
        ~Application();

        CLIENT::ClientManager *getClientManager();
        CLIENT::SubscribeManager *getSubscribeManager();
        CLIENT::ClientAbstract *getDefaultClient();
        void start();

    protected:
        ExceptionHandler *getExceptionHandler();
        shared_ptr<spdlog::logger> getLogger();
        string getAppPath();
        string getRuntimePath();
        void initAppEnv();

        void registerClient();
        void startSubscribe();

        virtual void beforeStart();
        virtual void afterStart();

    protected:
        CONFIG::Config *config;
        shared_ptr<spdlog::logger> logger;
        ExceptionHandler *exceptionHandler = nullptr;
        IOT::CLIENT::SubscribeManager *subscribeManager = nullptr;
        CLIENT::ClientManager *clientManager = nullptr;
    };
}

#endif //ARM_IOT_APPLICATION_H
