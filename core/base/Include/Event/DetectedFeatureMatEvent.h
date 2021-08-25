//
// Created by rxwyun on 2021/8/25.
//

#ifndef ARM_FACE_IDENTIFY_DETECTEDFEATUREMATEVENT_H
#define ARM_FACE_IDENTIFY_DETECTEDFEATUREMATEVENT_H

#include "./BaseEvent.h"

namespace ArmFaceIdentify {
    class DetectedFeatureMatEvent : public BaseEvent {
    public:
        Mat sourceMat;
        Mat tmpMat;
        Rect face;
        DetectedFeatureMatEvent(Mat sourceMat, Mat tmpMat, Rect face) : sourceMat(sourceMat), tmpMat(tmpMat), face(face) {}
    };
}

#endif //ARM_FACE_IDENTIFY_DETECTEDFEATUREMATEVENT_H
