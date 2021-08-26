//
// Created by rxwyun on 2021/8/26.
//

#ifndef ARM_FACE_IDENTIFY_DETECTEDFACE_H
#define ARM_FACE_IDENTIFY_DETECTEDFACE_H

#include "opencv2/core/mat.hpp"

using namespace cv;

namespace ArmFaceIdentify {
    class DetectedFace {
    public:
        Mat sourceMat;
        Mat detectMat;
        Rect face;

        DetectedFace(Mat sourceMat, Mat detectMat, Rect face) : sourceMat(sourceMat), detectMat(detectMat), face(face) {}
    };
}

#endif //ARM_FACE_IDENTIFY_DETECTEDFACE_H
