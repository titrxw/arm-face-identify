//
// Created by rxwyun on 2022/7/13.
//

#include "Identify.h"

Identify::Identify(const string& cascadeFilePath, vector<string> modelFilesPath) : cascadeFilePath(cascadeFilePath), modelFilesPath(modelFilesPath) {
    this->eventDispatcher = new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)>();
}

void Identify::setPredictMatConfidence(double predictMatConfidence) {
    this->predictMatConfidence = predictMatConfidence;
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

ArmFaceIdentify::DialogVideoFaceIdentify *Identify::getFaceIdentifyHandler() {
    if (this->faceIdentifyHandler == nullptr) {
        this->faceIdentifyHandler = new ArmFaceIdentify::DialogVideoFaceIdentify(this->getCascadeClassifier(), this->getModelRecognizer(), this->eventDispatcher);
        this->faceIdentifyHandler->setPredictMatMapValidator([this](vector<ArmFaceIdentify::PredictMat> predictMatMap) {
            if (predictMatMap.size() > 1) {
                return false;
            }

            if (predictMatMap[0].confidence < this->predictMatConfidence){
                return true;
            }
        });
    }

    return this->faceIdentifyHandler;
}

vector<ArmFaceIdentify::PredictMat> Identify::startIdentifyFromVideoCapture(VideoCapture *vc) {
    vector<ArmFaceIdentify::PredictMat> predictMatMap = this->getFaceIdentifyHandler()->identifyFromVideoCapture(vc);
    vc->release();

    return predictMatMap;
}

Identify::~Identify() {
    if (this->faceIdentifyHandler != nullptr) {
        delete this->faceIdentifyHandler;
    }
}
