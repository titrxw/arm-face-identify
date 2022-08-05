//
// Created by rxwyun on 2022/7/13.
//

#include "Train.h"

Train::Train(const string& cascadeFilePath, string targetDir) : cascadeFilePath(cascadeFilePath), targetDir(targetDir) {
    this->eventDispatcher = new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)>();
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

Train::~Train() {
    if (this->modelRecognizer) {
        this->modelRecognizer.release();
    }
    if (this->cascade) {
        this->cascade.release();
    }

    delete this->eventDispatcher;
    this->eventDispatcher = nullptr;

    if (this->faceTrainHandler != nullptr) {
        delete this->faceTrainHandler;
        this->faceTrainHandler = nullptr;
    }
}