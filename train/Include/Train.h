//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_TRAIN_H
#define ARM_FACE_IDENTIFY_TRAIN_H

#include <sstream>
#include "opencv2/core/mat.hpp"

using namespace cv;
using namespace std;

namespace ArmFaceIdentify {
    class Train {
    public:
        Train(){}
        void trainAndSave(const string& sourceFile, const string& targetFile);
        ~Train() {}
    protected:
        virtual void loadSourceFile(const string& filename, vector<Mat> &mats, vector<int> &matLabels, char separator = ';');
        virtual void trainMats(vector<Mat> &mats, vector<int> &matLabels, const string& targetFile);
    };
}



#endif //ARM_FACE_IDENTIFY_TRAIN_H
