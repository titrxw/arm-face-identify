//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_FACETRAIN_H
#define ARM_FACE_IDENTIFY_FACETRAIN_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "Train.h"

namespace ArmFaceIdentify
{
    class FaceTrain : public Train
    {
    public:
        FaceTrain(Ptr<FaceRecognizer> modelRecognizer) : Train(modelRecognizer) {}
        ~FaceTrain() {}

    protected:
        void loadSourceFile(const string &filename, vector<Mat> &mats, vector<int> &matLabels, char separator = ';');
        void trainMats(vector<Mat> &mats, vector<int> &matLabels, const string &targetFile);
    };
}

#endif //ARM_FACE_IDENTIFY_FACETRAIN_H
