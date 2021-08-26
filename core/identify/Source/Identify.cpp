#include "../Include/Identify.h"
#include "../../base/Enum/Event.h"
#include "../../base/Include/Event/PredictFeatureMatEvent.h"

ArmFaceIdentify::Identify::Identify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer) : cascade(cascade), Opencv(modelRecognizer){
}

vector<ArmFaceIdentify::PredictFace> ArmFaceIdentify::Identify::identify(Mat &model){
    vector<DetectedFace> detectedFaceMap = this->detectFaceMatFromMat(this->cascade, model);

    vector<PredictFace> predictFaceMap;
    for(int i = 0; i < detectedFaceMap.size(); i++) {
        PredictFace predictFace = this->predictMat(detectedFaceMap[i]);
        predictFaceMap.push_back(predictFace);

        if (this->eventDispatcher) {
            PredictFeatureMatEvent _predictMap(predictFace);
            this->eventDispatcher->dispatch(Event::PREDICT_FEATURE_IMAGE_FROM_FRAME, &_predictMap);
        }
    }
    detectedFaceMap.clear();

    return predictFaceMap;
}

ArmFaceIdentify::Identify::~Identify() {
    if (!this->cascade.empty()) {
        this->cascade.release();
    }
}
