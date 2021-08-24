#include "../Include/Identify.h"

ArmFaceIdentify::Identify::Identify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer) : cascade(cascade), modelRecognizer(modelRecognizer){
}

map<int, Mat> ArmFaceIdentify::Identify::identify(Mat model){
    model = this->pretreatmentMat(model);
    vector<Mat> pMats = this->detectMat(model);

    map<int, Mat> predictMap;
    for(int i = 0; i < pMats.size(); i++) {
        int predict = this->predictMat(pMats[i]);
        predictMap[predict] = pMats[i];
    }

    pMats.clear();

    return predictMap;
}

ArmFaceIdentify::Identify::~Identify() {
    if (!this->modelRecognizer.empty()) {
        this->modelRecognizer.release();
    }
    if (!this->cascade.empty()) {
        this->cascade.release();
    }
}
