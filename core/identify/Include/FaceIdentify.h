//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_FACE_IDENTIFY_H
#define ARM_FACE_IDENTIFY_FACE_IDENTIFY_H

#include <sstream>
#include "./Identify.h"

namespace ArmFaceIdentify {
    class FaceIdentify : public Identify {
    public:
        FaceIdentify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer) : Identify(cascade, modelRecognizer) {}
        ~FaceIdentify() {}

    protected:
        int predictMat(Mat model);
    };
}

#endif //ARM_FACE_IDENTIFY_FACE_IDENTIFY_H
