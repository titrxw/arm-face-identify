//
// Created by rxw on 2021/8/26.
//

#include "../Include/DialogVideoFaceIdentify.h"
#include "../../../core/base/Enum/Event.h"
#include "opencv2/highgui/highgui.hpp"

DialogVideoFaceIdentify::DialogVideoFaceIdentify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer,
                                                 VideoCapture *vc) : ArmFaceIdentify::FaceIdentify(cascade, modelRecognizer), vc(vc) {
    DialogVideoFaceIdentify::stopIdentity = false;

    this->setEventDispatcher(new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *)>());
    this->getEventDispatcher()->appendListener(ArmFaceIdentify::Event::DETECTED_FEATURE_IMAGE_FROM_FRAME, [this](ArmFaceIdentify::BaseEvent *event) {
        this->onDetectedFace((ArmFaceIdentify::DetectedFeatureMatEvent *)event);
    });
}

void DialogVideoFaceIdentify::onDetectedFace(ArmFaceIdentify::DetectedFeatureMatEvent *event) {
    rectangle(event->sourceMat, Point(event->face.x, event->face.y), Point(event->face.x + event->face.width, event->face.y + event->face.height),
              Scalar(0, 255, 0), 1, 8);
}

void DialogVideoFaceIdentify::identifyFromVideo() {
    Mat frame;
    while (this->vc->read(frame)) {
        if (frame.empty()) {
            break;
        }
        if (DialogVideoFaceIdentify::stopIdentity) {
            break;
        }

        this->identify(frame);

        imshow("arm_face_identify_dialog", frame);
    }
}

DialogVideoFaceIdentify::~DialogVideoFaceIdentify() {
    if (this->vc) {
        this->vc->release();
        delete this->vc;
        this->vc = nullptr;
    }
}
