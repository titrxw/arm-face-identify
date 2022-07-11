#include "../Include/FaceIdentify.h"

ArmFaceIdentify::PredictMat ArmFaceIdentify::FaceIdentify::predictMat(DetectedMat detectedFaceMap) {
    int label = -1;
    double confidence = 0;

    Mat tmpMat;
    resize(detectedFaceMap.detectMat, tmpMat, Size(92, 112));
    this->modelRecognizer->predict(tmpMat, label, confidence);

    return PredictMat(detectedFaceMap.sourceMat, detectedFaceMap.detectMat, detectedFaceMap.mat, label, confidence);
}
