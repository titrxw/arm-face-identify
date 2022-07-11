//
// Created by rxw on 2021/8/26.
//

#ifndef ARM_FACE_IDENTIFY_PREDICTMAT_H
#define ARM_FACE_IDENTIFY_PREDICTMAT_H

#include "opencv2/core/mat.hpp"
#include "./DetectedMat.h"

using namespace cv;

namespace ArmFaceIdentify {
    class PredictMat : public DetectedMat {
    public:
        int label;
        double confidence;

        PredictMat(Mat sourceMat, Mat detectMat, Rect mat, int label, double confidence = -1) : DetectedMat(sourceMat, detectMat, mat), label(label), confidence(confidence) {}
    };
}

#endif //ARM_FACE_IDENTIFY_PREDICTMAT_H
