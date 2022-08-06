//
// Created by rxw on 2021/8/26.
//

#include "../Include/DialogVideoFaceIdentify.h"
#include "../../base/Enum/Event.h"
#include "opencv2/highgui/highgui.hpp"
#include "../../helper/Include/Str.h"


ArmFaceIdentify::DialogVideoFaceIdentify::DialogVideoFaceIdentify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer,
                                                 EventDispatcher<int, void(
        ArmFaceIdentify::BaseEvent *)> *eventDispatcher) : ArmFaceIdentify::FaceIdentify(cascade, modelRecognizer, eventDispatcher) {
    this->getEventDispatcher()->appendListener(ArmFaceIdentify::Event::DETECTED_FEATURE_IMAGE_FROM_FRAME, [this](ArmFaceIdentify::BaseEvent *event) {
        this->onDetectedFaceListener((ArmFaceIdentify::DetectedFeatureMatEvent *)event);
    });
    this->getEventDispatcher()->appendListener(ArmFaceIdentify::Event::PREDICT_FEATURE_IMAGE_FROM_FRAME, [this](ArmFaceIdentify::BaseEvent *event) {
        this->onPredictFaceListener((ArmFaceIdentify::PredictFeatureMatEvent *)event);
    });
}

void ArmFaceIdentify::DialogVideoFaceIdentify::onDetectedFaceListener(ArmFaceIdentify::DetectedFeatureMatEvent *event) {
    rectangle(event->detectedFace.sourceMat, Point(event->detectedFace.mat.x, event->detectedFace.mat.y), Point(event->detectedFace.mat.x + event->detectedFace.mat.width, event->detectedFace.mat.y + event->detectedFace.mat.height),
              Scalar(0, 255, 0), 1, 8);
}

void ArmFaceIdentify::DialogVideoFaceIdentify::onPredictFaceListener(ArmFaceIdentify::PredictFeatureMatEvent *event) {
    string confidence = ArmFaceIdentify::Str::toString(event->predictFace.confidence);
    string label = ArmFaceIdentify::Str::toString(event->predictFace.label).append(" --- ").append(confidence);
    putText(event->predictFace.sourceMat, label, event->predictFace.mat.tl(), FONT_HERSHEY_COMPLEX, 1.2,  (0, 0, 255), 2, 0);
}

void ArmFaceIdentify::DialogVideoFaceIdentify::setPredictMatMapCallback(std::function<void (vector<ArmFaceIdentify::PredictMat>, string)> callback) {
    this->predictMatMapCallback = callback;
}

void ArmFaceIdentify::DialogVideoFaceIdentify::setCanIdentifyNextMatWithFlag(string flag) {
    this->canIdentifyNextMatFlag = flag;
}

bool ArmFaceIdentify::DialogVideoFaceIdentify::isCanIdentifyNextMat() const {
    return !this->canIdentifyNextMatFlag.empty();
}

void ArmFaceIdentify::DialogVideoFaceIdentify::stopIdentifyFromVideo() {
    this->isStopIdentify = true;
}

bool ArmFaceIdentify::DialogVideoFaceIdentify::ifNecessaryStop() {
    return waitKey(10) == 'k' || this->isStopIdentify;
}

void ArmFaceIdentify::DialogVideoFaceIdentify::identifyFromVideoCapture(VideoCapture *vc) {
    Mat frame;
    vector<ArmFaceIdentify::PredictMat> predictFaceMap;

    while (vc->read(frame)) {
        if (this->ifNecessaryStop()) {
            this->isStopIdentify = false;
            break;
        }

        if (frame.empty()) {
            throw "video capture read frame empty";
        }

        predictFaceMap = this->identifyMat(frame);
        imshow(DialogVideoFaceIdentify::DIALOG_NAME, frame);

        if (!this->isCanIdentifyNextMat() || predictFaceMap.size() == 0) {
            continue;
        }

        if (this->predictMatMapCallback == nullptr) {
            break;
        }
        if (this->predictMatMapCallback != nullptr) {
            string flag = this->canIdentifyNextMatFlag;
            this->setCanIdentifyNextMatWithFlag("");
            this->predictMatMapCallback(predictFaceMap, flag);
        }
    }
    cvDestroyWindow(DialogVideoFaceIdentify::DIALOG_NAME.c_str());
    predictFaceMap.clear();
}

ArmFaceIdentify::DialogVideoFaceIdentify::~DialogVideoFaceIdentify() {

}
