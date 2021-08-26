#include "../Include/FaceIdentify.h"

ArmFaceIdentify::PredictFace ArmFaceIdentify::FaceIdentify::predictMat(Mat model) {
    int label = -1;
    double confidence = 0;
    resize(model, model, Size(92, 112));
    if (!model.empty()) {
        this->modelRecognizer->predict(model, label, confidence);
    }

    return PredictFace(model, label, confidence);
}
