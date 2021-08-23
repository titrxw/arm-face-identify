//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_FACE_IDENTIFY_H
#define ARM_FACE_IDENTIFY_FACE_IDENTIFY_H

#include <sstream>
#include "./Identify.h"

using namespace std;

namespace ArmFaceIdentify
{
    class FaceIdentify : public Identify
    {
    public:
        using Identify::Identify;
        ~FaceIdentify() {}

    protected:
        Mat *detectMat(Mat model);
        int predictMat(Mat model);
        Mat pretreatmentMat(Mat model);
    };
}

#endif //ARM_FACE_IDENTIFY_FACE_IDENTIFY_H
