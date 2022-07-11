//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_IDENTIFY_H
#define ARM_FACE_IDENTIFY_IDENTIFY_H

#include "../../base/Include/Opencv.h"
#include "../../entity/Include/DetectedMat.h"
#include "../../entity/Include/PredictMat.h"

namespace ArmFaceIdentify {
    class Identify : public Opencv {
    public:
        Identify(Ptr<CascadeClassifier> cascade, EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)> *eventDispatcher = nullptr);
        ~Identify();

        vector<PredictMat> identifyMat(Mat &model);

    protected:
        Ptr<CascadeClassifier> cascade;

        virtual PredictMat predictMat(DetectedMat detectedMatMap) = 0;
    };
}

#endif //ARM_FACE_IDENTIFY_IDENTIFY_H
