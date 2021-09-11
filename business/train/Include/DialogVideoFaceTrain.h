//
// Created by rxwyun on 2021/8/24.
//

#ifndef ARM_FACE_IDENTIFY_DIALOGVIDEOFACETRAIN_H
#define ARM_FACE_IDENTIFY_DIALOGVIDEOFACETRAIN_H

#include "../../../core/train/Include/FaceTrain.h"
#include "../../../core/base/Include/Event/DetectedFeatureMatEvent.h"

class DialogVideoFaceTrain : public ArmFaceIdentify::FaceTrain {
public:
    DialogVideoFaceTrain(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer, EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)> *eventDispatcher, VideoCapture *vc, const string& targetDir);
    ~DialogVideoFaceTrain();

    void train(unsigned int label);

protected:
    Ptr<CascadeClassifier> cascade;
    VideoCapture *vc = nullptr;
    const string &targetDir;

    const int DETECTED_FACE_NUM = 30;
    const string SAMPLE_FILE_NAME = "arm_face_sample.txt";
    const string SAMPLE_FILE_TRAIN_NAME = "arm_face_sample_train.xml";

    string makeSampleFile(unsigned int label);
    void onDetectedFaceListener(ArmFaceIdentify::DetectedFeatureMatEvent *event);
};

#endif //ARM_FACE_IDENTIFY_DIALOGVIDEOFACETRAIN_H
