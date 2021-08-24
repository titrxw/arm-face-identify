//
// Created by rxwyun on 2021/8/24.
//

#ifndef ARM_FACE_IDENTIFY_OPENCV_H
#define ARM_FACE_IDENTIFY_OPENCV_H

#include "vector"
#include <map>
#include <opencv2/opencv.hpp>
#include "opencv2/core/mat.hpp"

using namespace cv;
using namespace std;

namespace ArmFaceIdentify {
    class Opencv {
    public:
        Opencv() {}
        ~Opencv() {}
    protected:
        static Mat pretreatmentMat(Mat &model);
        static vector<Mat> getFaceMatFromMat(Ptr<CascadeClassifier> cascade, Mat &model);
    };
}

#endif //ARM_FACE_IDENTIFY_OPENCV_H
