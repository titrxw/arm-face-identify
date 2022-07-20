//
// Created by rxwyun on 2022/7/13.
//

#include "Train.h"

Train::Train(string cascadeFilePath, string targetDir) : cascadeFilePath(cascadeFilePath), targetDir(targetDir) {
    this->eventDispatcher = new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)>();
}

Train::~Train() {
    if (this->faceTrainHandler != nullptr) {
        delete this->faceTrainHandler;
    }
}

Ptr<CascadeClassifier> Train::getCascadeClassifier() {
    if (this->cascade == nullptr) {
        this->cascade = new CascadeClassifier(this->cascadeFilePath);
    }

    return this->cascade;
}

Ptr<FaceRecognizer> Train::getModelRecognizer() {
    if (this->modelRecognizer == nullptr) {
        this->modelRecognizer = EigenFaceRecognizer::create();
    }

    return this->modelRecognizer;
}

ArmFaceIdentify::DialogVideoFaceTrain *Train::getFaceTrainHandler() {
    if (this->faceTrainHandler == nullptr) {
        this->faceTrainHandler = new ArmFaceIdentify::DialogVideoFaceTrain(this->getCascadeClassifier(), this->getModelRecognizer(), this->eventDispatcher, this->targetDir);
    }

    return this->faceTrainHandler;
}

string Train::trainFromRemoteImgUrls(vector<string> remoteImgUrls) {
    return std::string();
}
