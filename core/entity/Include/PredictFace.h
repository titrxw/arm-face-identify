//
// Created by rxw on 2021/8/26.
//

#ifndef ARM_FACE_IDENTIFY_PREDICTFACE_H
#define ARM_FACE_IDENTIFY_PREDICTFACE_H

#include "opencv2/core/mat.hpp"

using namespace cv;

namespace ArmFaceIdentify {
    class PredictFace {
    public:
        Mat model;
        int label;
        double confidence;

        PredictFace(Mat model, int label, double confidence) : model(model), label(label), confidence(confidence) {}
    };
}

#endif //ARM_FACE_IDENTIFY_PREDICTFACE_H
