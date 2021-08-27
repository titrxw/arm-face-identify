//
// Created by rxw on 2021/8/26.
//

#include "../Include/DialogVideoFaceIdentify.h"
#include "../../../core/base/Enum/Event.h"
#include "opencv2/highgui/highgui.hpp"
#include "../../../core/helper/Include/Str.h"

DialogVideoFaceIdentify::DialogVideoFaceIdentify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer,
                                                 VideoCapture *vc) : ArmFaceIdentify::FaceIdentify(cascade, modelRecognizer), vc(vc) {
    this->setEventDispatcher(new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *)>());
    this->getEventDispatcher()->appendListener(ArmFaceIdentify::Event::DETECTED_FEATURE_IMAGE_FROM_FRAME, [this](ArmFaceIdentify::BaseEvent *event) {
        this->onDetectedFaceListener((ArmFaceIdentify::DetectedFeatureMatEvent *)event);
    });
    this->getEventDispatcher()->appendListener(ArmFaceIdentify::Event::PREDICT_FEATURE_IMAGE_FROM_FRAME, [this](ArmFaceIdentify::BaseEvent *event) {
        this->onPredictFaceListener((ArmFaceIdentify::PredictFeatureMatEvent *)event);
    });
}

void DialogVideoFaceIdentify::onDetectedFaceListener(ArmFaceIdentify::DetectedFeatureMatEvent *event) {
    rectangle(event->detectedFace.sourceMat, Point(event->detectedFace.face.x, event->detectedFace.face.y), Point(event->detectedFace.face.x + event->detectedFace.face.width, event->detectedFace.face.y + event->detectedFace.face.height),
              Scalar(0, 255, 0), 1, 8);
}

void DialogVideoFaceIdentify::onPredictFaceListener(ArmFaceIdentify::PredictFeatureMatEvent *event) {
    string confidence = ArmFaceIdentify::Str::toString(event->predictFace.confidence);
    string label = ArmFaceIdentify::Str::toString(event->predictFace.label).append(" --- ").append(confidence);
    putText(event->predictFace.sourceMat, label, event->predictFace.face.tl(), FONT_HERSHEY_COMPLEX, 1.2,  (0, 0, 255), 2, 0);
}

void DialogVideoFaceIdentify::identifyFromVideo() {
    Mat frame;
    string dialogName = "arm_face_identify_dialog";
    while (this->vc->read(frame)) {
        if (frame.empty()) {
            break;
        }
        if (DialogVideoFaceIdentify::ifNecessaryStop()) {
            break;
        }

        this->identify(frame);

        imshow(dialogName, frame);
    }
    cvDestroyWindow(dialogName.c_str());
}

DialogVideoFaceIdentify::~DialogVideoFaceIdentify() {
    if (this->vc) {
        this->vc->release();
        this->vc = nullptr;
    }
}

bool DialogVideoFaceIdentify::ifNecessaryStop() {
    return waitKey(10) == 'k';
}
