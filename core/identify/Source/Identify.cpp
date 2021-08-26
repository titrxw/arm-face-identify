#include "../Include/Identify.h"

ArmFaceIdentify::Identify::Identify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer) : cascade(cascade), Opencv(modelRecognizer){
}

vector<ArmFaceIdentify::PredictFace> ArmFaceIdentify::Identify::identify(Mat &model){
    Mat tmpModel = ArmFaceIdentify::Identify::pretreatmentMat(model);
    vector<Mat> pMats = this->getFaceMatFromMat(this->cascade, model);

    vector<PredictFace> predictMap;
    for(int i = 0; i < pMats.size(); i++) {
        predictMap.push_back(this->predictMat(pMats[i]));
    }
    pMats.clear();

    return predictMap;
}

ArmFaceIdentify::Identify::~Identify() {
    if (!this->cascade.empty()) {
        this->cascade.release();
    }
}
