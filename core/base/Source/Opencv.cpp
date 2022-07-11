//
// Created by rxwyun on 2021/8/24.
//

#include "../Include/Opencv.h"
#include "../Enum/Event.h"
#include "../Include/Event/DetectedFeatureMatEvent.h"

Mat ArmFaceIdentify::Opencv::pretreatmentMat(Mat model) {
    Mat grayMat;
    cvtColor(model, grayMat, CV_RGB2GRAY); //测试图像必须为灰度图
    equalizeHist(grayMat, grayMat);      //变换后的图像进行直方图均值化处理

    return grayMat;
}

vector<ArmFaceIdentify::DetectedMat> ArmFaceIdentify::Opencv::detectFaceMatFromMat(Ptr<CascadeClassifier> cascade, Mat &model) {
    Mat grayMat = Opencv::pretreatmentMat(model);
    vector<Rect> faces; //建立用于存放人脸的向量容器
    cascade->detectMultiScale(grayMat, faces,
                                    1.1, 5, 0,
                              Size(80, 80), Size(500, 500));

    Mat detectMat;
    vector<DetectedMat> detectedFaceMap;
    for (int i = 0; i < faces.size(); i++) {
        detectMat = grayMat(faces[i]); //将所有的脸部保存起来
        if (detectMat.empty())
            continue;

        DetectedMat detectedFace(model, detectMat, faces[i]);
        detectedFaceMap.push_back(detectedFace);

        if (this->eventDispatcher) {
            DetectedFeatureMatEvent event(detectedFace);
            this->eventDispatcher->dispatch(Event::DETECTED_FEATURE_IMAGE_FROM_FRAME, &event);
        }
    }
    faces.clear();

    return detectedFaceMap;
}

ArmFaceIdentify::Opencv::~Opencv() {
    if (!this->modelRecognizer.empty()) {
        this->modelRecognizer.release();
    }
    if (this->eventDispatcher) {
        delete this->eventDispatcher;
        this->eventDispatcher = nullptr;
    }
}

void ArmFaceIdentify::Opencv::setModelRecognizer(Ptr<FaceRecognizer> modelRecognizer) {
    this->modelRecognizer = modelRecognizer;
}

Ptr<FaceRecognizer> ArmFaceIdentify::Opencv::getModelRecognizer() {
    return this->modelRecognizer;
}

void ArmFaceIdentify::Opencv::setEventDispatcher(EventDispatcher<int, void(BaseEvent *)> *eventDispatcher) {
    this->eventDispatcher = eventDispatcher;
}

EventDispatcher<int, void(ArmFaceIdentify::BaseEvent *)> *ArmFaceIdentify::Opencv::getEventDispatcher() {
    return this->eventDispatcher;
}
