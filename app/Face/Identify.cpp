//
// Created by rxwyun on 2022/7/13.
//

#include "Identify.h"
#include "../../app_framework/Util/Filesystem.hpp"

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
        this->modelRecognizer = LBPHFaceRecognizer::create();
        this->reloadModelRecognizer();
    }

    return this->modelRecognizer;
}

void Identify::reloadModelRecognizer() {
    this->modelRecognizer->clear();

    vector<string>tmp;
    int size = this->modelFilesPath.size();

    for (int i = 0; i < size; i++) {
        if (Filesystem::fileExists(this->modelFilesPath[i])) {
            this->modelRecognizer->read(this->modelFilesPath[i]);
            tmp.push_back(this->modelFilesPath[i]);
        }
    }

    this->modelFilesPath.clear();
    this->modelFilesPath = tmp;
}

void Identify::setPredictMatMapCallback(std::function<void (ArmFaceIdentify::PredictMat, string)> callback) {
    this->predictMatCallback = callback;
}

ArmFaceIdentify::DialogVideoFaceIdentify *Identify::getFaceIdentifyHandler() {
    if (this->faceIdentifyHandler == nullptr) {
        this->faceIdentifyHandler = new ArmFaceIdentify::DialogVideoFaceIdentify(this->getCascadeClassifier(), this->getModelRecognizer(), this->eventDispatcher);
        this->faceIdentifyHandler->setPredictMatMapCallback([this](vector<ArmFaceIdentify::PredictMat> predictMatMap, string flag) {
            if (predictMatMap.size() > 1) {
                return;
            }

            if (predictMatMap[0].confidence > this->predictMatConfidence){
                return;
            }

            time_t curTime = time(NULL);
            if (this->curIdentifyLabel == predictMatMap[0].label && abs(curTime - this->curIdentifyLatestTime) < 60) {
                return;
            }

            this->curIdentifyLabel = predictMatMap[0].label;
            this->curIdentifyLatestTime = curTime;
            if (this->predictMatCallback != nullptr) {
                this->predictMatCallback(predictMatMap[0], flag);
            }
        });
    }

    return this->faceIdentifyHandler;
}

void Identify::startIdentifyFromVideoCapture() {
    VideoCapture capture(this->videoCaptureIndex);
    this->getFaceIdentifyHandler()->identifyFromVideoCapture(&capture);
    capture.release();
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
