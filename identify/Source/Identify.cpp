#include "../Include/Identify.h"

ArmFaceIdentify::Identify::Identify(CascadeClassifier cascade, Ptr<FaceRecognizer> modelRecognizer) : cascade(cascade), modelRecognizer(modelRecognizer){
}

int ArmFaceIdentify::Identify::identify(Mat model){
    model = this->pretreatmentMat(model);
    std::vector<Mat> pMats = this->detectMat(model);

    for (int i = 0; i < pMats.size(); i++)
    {
        int predict = this->predictMat(pMats[i]);
    }

    return 1;
}
