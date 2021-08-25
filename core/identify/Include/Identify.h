//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_IDENTIFY_H
#define ARM_FACE_IDENTIFY_IDENTIFY_H

#include "../../base/Include/Opencv.h"

namespace ArmFaceIdentify {
    class Identify : public Opencv {
    public:
        Identify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer);
        map<int, Mat> identify(Mat &model);
        ~Identify();

    protected:
        Ptr<CascadeClassifier> cascade;

        virtual int predictMat(Mat model) = 0;
    };
}

#endif //ARM_FACE_IDENTIFY_IDENTIFY_H
