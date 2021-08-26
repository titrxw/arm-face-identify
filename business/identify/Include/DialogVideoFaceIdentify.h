//
// Created by rxw on 2021/8/26.
//

#ifndef ARM_FACE_IDENTIFY_DIALOGVIDEOFACEIDENTIFY_H
#define ARM_FACE_IDENTIFY_DIALOGVIDEOFACEIDENTIFY_H

#include "../../../core/identify/Include/FaceIdentify.h"
#include "../../../core/base/Include/Event/DetectedFeatureMatEvent.h"

class DialogVideoFaceIdentify : public ArmFaceIdentify::FaceIdentify {
public:
    DialogVideoFaceIdentify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer, VideoCapture *vc);
    ~DialogVideoFaceIdentify();

    static bool stopIdentity;

    void identifyFromVideo();
    void onDetectedFace(ArmFaceIdentify::DetectedFeatureMatEvent *event);

protected:
    VideoCapture *vc = nullptr;
};


#endif //ARM_FACE_IDENTIFY_DIALOGVIDEOFACEIDENTIFY_H
