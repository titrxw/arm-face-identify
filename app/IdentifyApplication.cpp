//
// Created by rxwyun on 2022/7/23.
//

#include "IdentifyApplication.h"
#include "./Mqtt/DeviceCtrlSubscribe.hpp"

IdentifyApplication::IdentifyApplication(Config *config) : Application(config) {

}

Identify *IdentifyApplication::getFaceIdentifyHandler() {
    if (this->identify == nullptr) {
        this->identify = new Identify(this->getAppPath() + FACE_CASCADE_PATH, this->getAppPath() + FACE_MODEL_DIR, this->config->faceIdentify.videoCaptureIndex);
        this->identify->setPredictMatConfidence(this->config->faceIdentify.predictMatConfidence);
    }

    return this->identify;
}

void IdentifyApplication::registerSubscribe() {
    this->getSubscribeManager()->registerSubscriber(new DeviceCtrlSubscribe(this->config->device, this->getFaceIdentifyHandler(), this->getDefaultClient(),
                                                                            this->config->server.httpServerAddress));
}

void IdentifyApplication::beforeStart() {
    Application::beforeStart();

    this->registerSubscribe();
}

void IdentifyApplication::afterStart() {
    Application::afterStart();

    this->getFaceIdentifyHandler()->startIdentifyFromVideoCapture();
}

IdentifyApplication::~IdentifyApplication() {
    if (this->identify != nullptr) {
        delete this->identify;
        this->identify = nullptr;
    }
}
