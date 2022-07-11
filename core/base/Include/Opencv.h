//
// Created by rxwyun on 2021/8/24.
//

#ifndef ARM_FACE_IDENTIFY_OPENCV_H
#define ARM_FACE_IDENTIFY_OPENCV_H

#include "vector"
#include <map>
#include "opencv2/opencv.hpp"
#include "opencv2/face.hpp"
#include "opencv2/core/mat.hpp"
#include "eventpp/eventdispatcher.h"
#include "./Event/BaseEvent.h"
#include "../../entity/Include/DetectedMat.h"

using namespace cv;
using namespace cv::face;
using namespace std;
using namespace eventpp;

namespace ArmFaceIdentify {
    class Opencv {
    public:
        Opencv(EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)> *eventDispatcher = nullptr) : eventDispatcher(eventDispatcher) {}
        ~Opencv();

        void setEventDispatcher(EventDispatcher<int, void (BaseEvent *event)> *eventDispatcher);
        EventDispatcher<int, void (BaseEvent *event)> *getEventDispatcher();
    protected:
        EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)> *eventDispatcher = nullptr;

        static Mat pretreatmentMat(Mat model);
        vector<DetectedMat> detectFaceMatFromMat(Ptr<CascadeClassifier> cascade, Mat &model);
    };
}

#endif //ARM_FACE_IDENTIFY_OPENCV_H
