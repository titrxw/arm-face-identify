//
// Created by rxwyun on 2022/7/13.
//

#include "Identify.h"

Identify::Identify(string cascadeFilePath, vector<string> modelFilesPath) : cascadeFilePath(cascadeFilePath), modelFilesPath(modelFilesPath) {
    this->eventDispatcher = new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)>();
}

Identify::~Identify() {
    if (this->faceIdentifyHandler != nullptr) {
        delete this->faceIdentifyHandler;
    }
}

Ptr<CascadeClassifier> Identify::getCascadeClassifier() {
    if (this->cascade == nullptr) {
        this->cascade = new CascadeClassifier(this->cascadeFilePath);
    }

    return this->cascade;
}

Ptr<FaceRecognizer> Identify::getModelRecognizer() {
    if (this->modelRecognizer == nullptr) {
        this->modelRecognizer = EigenFaceRecognizer::create();
    }

    return this->modelRecognizer;
}

void Identify::startIdentifyFromVideoCapture(VideoCapture *vc) {

}

ArmFaceIdentify::DialogVideoFaceIdentify *Identify::getFaceIdentifyHandler() {
    if (this->faceIdentifyHandler == nullptr) {
        this->faceIdentifyHandler = new ArmFaceIdentify::DialogVideoFaceIdentify(this->getCascadeClassifier(), this->getModelRecognizer(), this->eventDispatcher);
    }

    return this->faceIdentifyHandler;
}
