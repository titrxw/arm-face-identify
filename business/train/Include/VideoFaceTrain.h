//
// Created by rxwyun on 2021/8/24.
//

#ifndef ARM_FACE_IDENTIFY_VIDEOFACETRAIN_H
#define ARM_FACE_IDENTIFY_VIDEOFACETRAIN_H

#include "../../../core/train/Include/FaceTrain.h"

class VideoFaceTrain : public ArmFaceIdentify::FaceTrain {
public:
    VideoFaceTrain(Ptr<FaceRecognizer> modelRecognizer) : ArmFaceIdentify::FaceTrain(modelRecognizer) {}
    ~VideoFaceTrain() {}

    string getSampleVideoCapture(VideoCapture *vc, Ptr<CascadeClassifier> cascade, unsigned int label, string& targetDir);
};

#endif //ARM_FACE_IDENTIFY_VIDEOFACETRAIN_H
