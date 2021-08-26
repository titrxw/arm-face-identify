#include "../Include/FaceIdentify.h"

ArmFaceIdentify::PredictFace ArmFaceIdentify::FaceIdentify::predictMat(DetectedFace detectedFaceMap) {
    int label = -1;
    double confidence = 0;

    Mat tmpMat;
    resize(detectedFaceMap.detectMat, tmpMat, Size(92, 112));
    this->modelRecognizer->predict(tmpMat, label, confidence);

    return PredictFace(detectedFaceMap.sourceMat, detectedFaceMap.detectMat, detectedFaceMap.face, label, confidence);
}
