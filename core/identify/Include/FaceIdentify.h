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
        FaceIdentify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer, EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)> *eventDispatcher = nullptr);
        ~FaceIdentify();

    protected:
        Ptr<FaceRecognizer> modelRecognizer;

        PredictMat predictMat(DetectedMat detectedFaceMap);
    };
}

#endif //ARM_FACE_IDENTIFY_FACE_IDENTIFY_H
