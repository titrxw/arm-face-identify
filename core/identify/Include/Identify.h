//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_IDENTIFY_H
#define ARM_FACE_IDENTIFY_IDENTIFY_H

#include "../../base/Include/Opencv.h"
#include "../../entity/Include/PredictFace.h"

namespace ArmFaceIdentify {
    class Identify : public Opencv {
    public:
        Identify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer);
        ~Identify();

        vector<PredictFace> identify(Mat &model);

    protected:
        Ptr<CascadeClassifier> cascade;

        virtual PredictFace predictMat(Mat model) = 0;
    };
}

#endif //ARM_FACE_IDENTIFY_IDENTIFY_H
