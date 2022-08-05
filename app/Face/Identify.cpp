//
// Created by rxwyun on 2022/7/13.
//

#include "Identify.h"

Identify::Identify(const string& cascadeFilePath, vector<string> modelFilesPath, int videoCaptureIndex) : cascadeFilePath(cascadeFilePath), modelFilesPath(modelFilesPath), videoCaptureIndex(videoCaptureIndex) {
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
        for (int i = 0; i < this->modelFilesPath.size(); i++) {
            this->modelRecognizer->read(this->modelFilesPath[i]);
        }
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

            return false;
        });
    }

    return this->faceIdentifyHandler;
}

vector<ArmFaceIdentify::PredictMat> Identify::startIdentifyFromVideoCapture() {
    VideoCapture capture(this->videoCaptureIndex);
    vector<ArmFaceIdentify::PredictMat> predictMatMap = this->getFaceIdentifyHandler()->identifyFromVideoCapture(&capture);
    capture.release();

    return predictMatMap;
}

Identify::~Identify() {
    if (this->cascade){
        this->cascade.release();
    }
    if (this->modelRecognizer) {
        this->modelRecognizer.release();
    }

    delete this->eventDispatcher;
    this->eventDispatcher = nullptr;

    if (this->faceIdentifyHandler != nullptr) {
        delete this->faceIdentifyHandler;
        this->faceIdentifyHandler = nullptr;
    }
}
