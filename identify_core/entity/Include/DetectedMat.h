//
// Created by rxwyun on 2021/8/26.
//

#ifndef ARM_FACE_IDENTIFY_DETECTEDMAT_H
#define ARM_FACE_IDENTIFY_DETECTEDMAT_H

#include "opencv2/core/mat.hpp"

using namespace cv;

namespace ArmFaceIdentify {
    class DetectedMat {
    public:
        Mat sourceMat;
        Mat detectMat;
        Rect mat;

        DetectedMat(Mat sourceMat, Mat detectMat, Rect mat) : sourceMat(sourceMat), detectMat(detectMat), mat(mat) {}
    };
}

#endif //ARM_FACE_IDENTIFY_DETECTEDMAT_H
