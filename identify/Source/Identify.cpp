#include "../Include/Identify.h"

ArmFaceIdentify::Identify::Identify(CascadeClassifier cascade, Ptr<BasicFaceRecognizer> modelRecognizer) : cascade(cascade), modelRecognizer(modelRecognizer)
{
}

int ArmFaceIdentify::Identify::identify(Mat model)
{
    model = this->pretreatmentMat(model);
    Mat *pMats = this->detectMat(model);

    for (int i = 0; i < pMats.size(); i++)
    {
        int predict = this->predictMat(pMats[i]);
    }

    delete[] pMats;
}