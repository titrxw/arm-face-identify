#include "../Include/FaceIdentify.h"

Mat ArmFaceIdentify::FaceIdentify::pretreatmentMat(Mat model)
{
    Mat grayModel;
    cvtColor(model, grayModel, CV_RGB2GRAY); //测试图像必须为灰度图
    equalizeHist(grayModel, grayModel);      //变换后的图像进行直方图均值化处理

    return grayModel;
}

Mat *ArmFaceIdentify::FaceIdentify::detectMat(Mat model)
{
    vector<Rect> faces(0); //建立用于存放人脸的向量容器
    this.cascade.detectMultiScale(model, faces,
                                  1.1, 4, 0
                                              //|CV_HAAR_FIND_BIGGEST_OBJECT
                                              | CV_HAAR_DO_ROUGH_SEARCH,
                                  //| CV_HAAR_SCALE_IMAGE,
                                  Size(30, 30), Size(500, 500));

    Mat *pMats = new Mat[faces.size()];
    for (int i = 0; i < faces.size(); i++)
    {
        pMats[i] = model(faces[i]); //将所有的脸部保存起来
        if (pMats[i].empty())
            continue;
        int predict = this->predictMat(pMats[i]);
    }

    return pMats;
}

int ArmFaceIdentify::FaceIdentify::predictMat(Mat model)
{
    int predict = 0;
    //截取的ROI人脸尺寸调整
    if (model.rows >= 120)
    {
        //改变图像大小，使用双线性差值
        resize(model, model, Size(92, 112));
    }
    //判断是否正确检测ROI
    if (!model.empty())
    {
        //测试图像应该是灰度图
        predict = this.modelRecognizer->predict(model);
    }

    return predict;
}