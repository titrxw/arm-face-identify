#include "../Include/FaceIdentify.h"

int ArmFaceIdentify::FaceIdentify::predictMat(Mat model) {
    int predict = -1;
    double confidence = 0;
    resize(model, model, Size(92, 112));
    if (!model.empty()) {
         this->modelRecognizer->predict(model, predict, confidence);
    }
    cout<<"label: "<<predict << "cd " << confidence<<endl;
    return predict;
}
