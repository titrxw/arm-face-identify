#include "../Include/Identify.h"
#include "../../base/Enum/Event.h"
#include "../../base/Include/Event/PredictFeatureMatEvent.h"

ArmFaceIdentify::Identify::Identify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer, EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)> *eventDispatcher) : cascade(cascade), Opencv(modelRecognizer, eventDispatcher){
}

vector<ArmFaceIdentify::PredictMat> ArmFaceIdentify::Identify::identifyMat(Mat &model){
    vector<DetectedMat> detectedMatMap = this->detectFaceMatFromMat(this->cascade, model);

    vector<PredictMat> predictMatMap;
    for(int i = 0; i < detectedMatMap.size(); i++) {
        PredictMat predictFace = this->predictMat(detectedMatMap[i]);
        predictMatMap.push_back(predictFace);

        if (this->eventDispatcher) {
            PredictFeatureMatEvent _predictMap(predictFace);
            this->eventDispatcher->dispatch(Event::PREDICT_FEATURE_IMAGE_FROM_FRAME, &_predictMap);
        }
    }
    detectedMatMap.clear();

    return predictMatMap;
}

ArmFaceIdentify::Identify::~Identify() {
    if (!this->cascade.empty()) {
        this->cascade.release();
    }
}
