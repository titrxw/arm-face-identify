#include "../Include/Identify.h"

ArmFaceIdentify::Identify::Identify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer) : cascade(cascade), modelRecognizer(modelRecognizer){
}

int ArmFaceIdentify::Identify::identify(Mat model){
    model = this->pretreatmentMat(model);
    vector<Mat> pMats = this->detectMat(model);

    for(int i = 0; i < pMats.size(); i++) {
        int predict = this->predictMat(pMats[i]);
    }

    pMats.clear();

    return 1;
}

ArmFaceIdentify::Identify::~Identify() {
    if (!this->modelRecognizer.empty()) {
        this->modelRecognizer.release();
    }
    if (!this->cascade.empty()) {
        this->cascade.release();
    }
}
