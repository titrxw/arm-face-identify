//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_TRAIN_H
#define ARM_FACE_IDENTIFY_TRAIN_H

#include <sstream>
#include "opencv2/face.hpp"
#include "../../base/Include/Opencv.h"

namespace ArmFaceIdentify {
    class Train : public Opencv {
    public:
        Train(Ptr<FaceRecognizer> modelRecognizer) : Opencv(modelRecognizer) {}
        ~Train() {}

        void trainAndSave(const string& sourceFile, const string& targetFile = "");
    protected:
        virtual void loadSourceFile(const string& filename, vector<Mat> &mats, vector<int> &matLabels, char separator = ';') = 0;
        virtual void trainMats(vector<Mat> &mats, vector<int> &matLabels, const string& targetFile) = 0;
    };
}

#endif //ARM_FACE_IDENTIFY_TRAIN_H