//
// Created by rxwyun on 2022/7/13.
//

#include "Train.h"
#include "../../app_framework/Util/Filesystem.hpp"

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
        this->modelRecognizer = LBPHFaceRecognizer::create();
    }

    return this->modelRecognizer;
}

ArmFaceIdentify::DialogVideoFaceTrain *Train::getFaceTrainHandler() {
    if (this->faceTrainHandler == nullptr) {
        this->faceTrainHandler = new ArmFaceIdentify::DialogVideoFaceTrain(this->getCascadeClassifier(), this->getModelRecognizer(), this->eventDispatcher, this->targetDir);
    }

    return this->faceTrainHandler;
}

string Train::getFaceModelSavePath(int label) {
    return this->targetDir  + "default_model.xml";
}

string Train::addFaceModelFromRemoteImgUrls(int label, vector<std::string> localPaths) {
    string labelStr = to_string(label);
    int localPathSize = localPaths.size();
    string modeFileContent;
    for (int i = 0; i < localPathSize; ++i) {
        modeFileContent = modeFileContent.append(localPaths[i]).append(";").append(labelStr).append("\n");
    }
    string samplePath = this->targetDir + labelStr + "_model.txt";
    Filesystem::write(samplePath, modeFileContent);

    string filePath = this->getFaceModelSavePath(label);
    this->getFaceTrainHandler()->trainAndSave(samplePath, filePath);
    Filesystem::unlink(samplePath);

    return filePath;
}

void Train::deleteFaceModel(int label) {
    string filePath = this->getFaceModelSavePath(label);
    if (Filesystem::fileExists(filePath)) {
        Filesystem::unlink(filePath);
    }
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
