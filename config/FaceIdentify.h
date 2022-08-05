//
// Created by rxw on 2022/8/5.
//

#ifndef ARM_FACE_IDENTIFY_FACEIDENTIFY_H
#define ARM_FACE_IDENTIFY_FACEIDENTIFY_H

#include <sstream>
#include "configor/configor.hpp"
#include "configor/json.hpp"

using namespace std;
using namespace configor;

class FaceIdentify {
public:
    int videoCaptureIndex;
    double predictMatConfidence;

    CONFIGOR_BIND(json, FaceIdentify, REQUIRED(videoCaptureIndex, "video_captcha_index"), REQUIRED(predictMatConfidence, "predict_mat_confidence"));
};

#endif //ARM_FACE_IDENTIFY_FACEIDENTIFY_H
