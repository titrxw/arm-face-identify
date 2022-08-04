//
// Created by rxwyun on 2021/8/26.
//

#ifndef ARM_FACE_IDENTIFY_PREDICTFEATUREMATEVENT_H
#define ARM_FACE_IDENTIFY_PREDICTFEATUREMATEVENT_H

#include "./BaseEvent.h"
#include "../../../entity/Include/PredictMat.h"

namespace ArmFaceIdentify {
    class PredictFeatureMatEvent : public BaseEvent {
    public:
        PredictMat predictFace;
        PredictFeatureMatEvent(PredictMat predictFace) : predictFace(predictFace) {}
    };
}

#endif //ARM_FACE_IDENTIFY_PREDICTFEATUREMATEVENT_H
