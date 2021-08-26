//
// Created by rxw on 2021/8/26.
//

#ifndef ARM_FACE_IDENTIFY_PREDICTFACE_H
#define ARM_FACE_IDENTIFY_PREDICTFACE_H

#include "opencv2/core/mat.hpp"
#include "./DetectedFace.h"

using namespace cv;

namespace ArmFaceIdentify {
    class PredictFace : public DetectedFace {
    public:
        int label;
        double confidence;

        PredictFace(Mat sourceMat, Mat detectMat, Rect face, int label, double confidence = -1) : DetectedFace(sourceMat, detectMat, face), label(label), confidence(confidence) {}
    };
}

#endif //ARM_FACE_IDENTIFY_PREDICTFACE_H
