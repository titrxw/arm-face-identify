//
// Created by rxwyun on 2021/8/22.
//

#include "../Include/Train.h"

void ArmFaceIdentify::Train::trainAndSave(const string& sourceFile, const string& targetFile) {
    vector<Mat> mats;
    vector<int> matLabels;
    this->loadSourceFile(sourceFile, mats, matLabels);
    this->trainMats(mats, matLabels, targetFile);
}