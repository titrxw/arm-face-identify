#include "../Include/FaceIdentify.h"

int ArmFaceIdentify::FaceIdentify::predictMat(Mat model) {
    int predict = -1;
    resize(model, model, Size(92, 112));
    if (!model.empty()) {
        predict = this->modelRecognizer->predict(model);
    }

    return predict;
}
