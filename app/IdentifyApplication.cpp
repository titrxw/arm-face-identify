//
// Created by rxwyun on 2022/7/23.
//

#include "IdentifyApplication.h"
#include "./Mqtt/DeviceCtrlSubscribe.h"

IdentifyApplication::IdentifyApplication(Config config) : Application(config) {

}

void IdentifyApplication::registerSubscribe() {
    this->getSubscribeManager()->registerSubscriber(new DeviceCtrlSubscribe(this->config.device));
}

void IdentifyApplication::beforeStart() {
    Application::beforeStart();

    this->registerSubscribe();
}

void IdentifyApplication::afterStart() {
    Application::afterStart();
}

IdentifyApplication::~IdentifyApplication() {

}
