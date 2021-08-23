//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_IDENTIFY_H
#define ARM_FACE_IDENTIFY_IDENTIFY_H

#include "opencv2/core/mat.hpp"
#include "opencv2/face.hpp"

using namespace cv;
using namespace cv::face;

namespace ArmFaceIdentify {
    class Identify {
    public:
        Identify(CascadeClassifier cascade, Ptr<FaceRecognizer> modelRecognizer);
        int identify(Mat model);
        ~Identify() {}

    protected:
        Ptr<FaceRecognizer> modelRecognizer;
        CascadeClassifier cascade;

        virtual std::vector<Mat> detectMat(Mat model) = 0;
        virtual int predictMat(Mat model) = 0;
        virtual Mat pretreatmentMat(Mat model) = 0;
    };
}

#endif //ARM_FACE_IDENTIFY_IDENTIFY_H
