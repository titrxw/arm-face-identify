//
// Created by rxwyun on 2021/8/25.
//

#ifndef ARM_FACE_IDENTIFY_DETECTEDFEATUREMATEVENT_H
#define ARM_FACE_IDENTIFY_DETECTEDFEATUREMATEVENT_H

#include "./BaseEvent.h"
#include "../../../entity/Include/DetectedFace.h"

namespace ArmFaceIdentify {
    class DetectedFeatureMatEvent : public BaseEvent {
    public:
        DetectedFace detectedFace;
        map<string, string> options;

        DetectedFeatureMatEvent(DetectedFace detectedFace, map<string, string> options = {}) : detectedFace(detectedFace), options(options) {}
    };
}

#endif //ARM_FACE_IDENTIFY_DETECTEDFEATUREMATEVENT_H
