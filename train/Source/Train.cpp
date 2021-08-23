//
// Created by rxwyun on 2021/8/22.
//

#include "../Include/Train.h"

Ptr<FaceRecognizer> ArmFaceIdentify::Train::trainAndSave(const string& sourceFile, const string& targetFile) {
    vector<Mat> mats;
    vector<int> matLabels;
    this->loadSourceFile(sourceFile, mats, matLabels);
    return this->trainMats(mats, matLabels, targetFile);
}