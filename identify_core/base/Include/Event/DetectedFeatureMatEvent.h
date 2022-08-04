//
// Created by rxwyun on 2021/8/25.
//

#ifndef ARM_FACE_IDENTIFY_DETECTEDFEATUREMATEVENT_H
#define ARM_FACE_IDENTIFY_DETECTEDFEATUREMATEVENT_H

#include "./BaseEvent.h"
#include "../../../entity/Include/DetectedMat.h"

namespace ArmFaceIdentify {
    class DetectedFeatureMatEvent : public BaseEvent {
    public:
        DetectedMat detectedFace;
        map<string, string> options;

        DetectedFeatureMatEvent(DetectedMat detectedFace, map<string, string> options = {}) : detectedFace(detectedFace), options(options) {}
    };
}

#endif //ARM_FACE_IDENTIFY_DETECTEDFEATUREMATEVENT_H
