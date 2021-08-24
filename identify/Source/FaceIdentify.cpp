#include "../Include/FaceIdentify.h"
#include <opencv2/opencv.hpp>

Mat ArmFaceIdentify::FaceIdentify::pretreatmentMat(Mat model) {
    cvtColor(model, model, CV_RGB2GRAY); //测试图像必须为灰度图
    equalizeHist(model, model);      //变换后的图像进行直方图均值化处理

    return model;
}

vector<Mat> ArmFaceIdentify::FaceIdentify::detectMat(Mat model) {
    vector<Rect> faces; //建立用于存放人脸的向量容器
    this->cascade->detectMultiScale(model, faces,
                                  1.1, 4, 0
                                              //|CV_HAAR_FIND_BIGGEST_OBJECT
                                              | CV_HAAR_DO_ROUGH_SEARCH,
                                  //| CV_HAAR_SCALE_IMAGE,
                                  Size(30, 30), Size(500, 500));

    Mat tmpModel;
    vector<Mat> pMats;
    for (int i = 0; i < faces.size(); i++) {
        tmpModel = model(faces[i]); //将所有的脸部保存起来
        if (tmpModel.empty())
            continue;
        pMats.push_back(tmpModel);
    }

    faces.clear();
    return pMats;
}

int ArmFaceIdentify::FaceIdentify::predictMat(Mat model) {
    int predict = -1;
    resize(model, model, Size(92, 112));
    if (!model.empty()) {
        predict = this->modelRecognizer->predict(model);
    }

    return predict;
}
