#include "../Include/Identify.h"

ArmFaceIdentify::Identify::Identify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer) : cascade(cascade), Opencv(modelRecognizer){
}

map<int, Mat> ArmFaceIdentify::Identify::identify(Mat &model){
    Mat tmpModel = ArmFaceIdentify::Identify::pretreatmentMat(model);
    vector<Mat> pMats = this->getFaceMatFromMat(this->cascade, model);

    map<int, Mat> predictMap;
    for(int i = 0; i < pMats.size(); i++) {
        int predict = this->predictMat(pMats[i]);
        predictMap[predict] = pMats[i];
    }

    pMats.clear();

    return predictMap;
}

ArmFaceIdentify::Identify::~Identify() {
    if (!this->cascade.empty()) {
        this->cascade.release();
    }
}
