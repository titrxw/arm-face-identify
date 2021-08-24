//
// Created by rxwyun on 2021/8/24.
//

#include "../Include/Opencv.h"
//#include<opencv2/highgui/highgui.hpp>

Mat ArmFaceIdentify::Opencv::pretreatmentMat(Mat &model) {
    cvtColor(model, model, CV_RGB2GRAY); //测试图像必须为灰度图
    equalizeHist(model, model);      //变换后的图像进行直方图均值化处理

    return model;
}

vector<Mat> ArmFaceIdentify::Opencv::getFaceMatFromMat(Ptr<CascadeClassifier> cascade, Mat &model) {
    vector<Rect> faces; //建立用于存放人脸的向量容器
    cascade->detectMultiScale(model, faces,
                                    1.1, 5, 0,
                              Size(80, 80), Size(500, 500));

    Mat tmpModel;
    vector<Mat> pMats;
    for (int i = 0; i < faces.size(); i++) {
        tmpModel = model(faces[i]); //将所有的脸部保存起来
        if (tmpModel.empty())
            continue;
//        rectangle(model, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
//                  Scalar(0, 255, 0), 1, 8);    //框出人脸位置
        pMats.push_back(tmpModel);
    }

    faces.clear();
    return pMats;
}
