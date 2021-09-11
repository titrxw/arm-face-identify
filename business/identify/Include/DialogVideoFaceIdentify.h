//
// Created by rxw on 2021/8/26.
//

#ifndef ARM_FACE_IDENTIFY_DIALOGVIDEOFACEIDENTIFY_H
#define ARM_FACE_IDENTIFY_DIALOGVIDEOFACEIDENTIFY_H

#include "../../../core/identify/Include/FaceIdentify.h"
#include "../../../core/base/Include/Event/DetectedFeatureMatEvent.h"
#include "../../../core/base/Include/Event/PredictFeatureMatEvent.h"

class DialogVideoFaceIdentify : public ArmFaceIdentify::FaceIdentify {
public:
    DialogVideoFaceIdentify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer, VideoCapture *vc, EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *)> *eventDispatcher);
    ~DialogVideoFaceIdentify();

    void identifyFromVideo();
    void onDetectedFaceListener(ArmFaceIdentify::DetectedFeatureMatEvent *event);
    void onPredictFaceListener(ArmFaceIdentify::PredictFeatureMatEvent *event);

protected:
    VideoCapture *vc = nullptr;

    static bool ifNecessaryStop();
};

#endif //ARM_FACE_IDENTIFY_DIALOGVIDEOFACEIDENTIFY_H
