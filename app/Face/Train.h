//
// Created by rxwyun on 2022/7/13.
//

#ifndef ARM_FACE_IDENTIFY_TRAIN_APP_H
#define ARM_FACE_IDENTIFY_TRAIN_APP_H

#include <sstream>
#include "vector"
#include "opencv2/face.hpp"
#include "../../identify_core/train/Include/DialogVideoFaceTrain.h"

using namespace cv;
using namespace cv::face;
using namespace std;

class Train
{
public:
    Train(const string& cascadeFilePath, string targetDir);
    ~Train();

    Ptr<CascadeClassifier> getCascadeClassifier();
    Ptr<FaceRecognizer> getModelRecognizer();
    ArmFaceIdentify::DialogVideoFaceTrain *getFaceTrainHandler();
    string trainFromRemoteImgUrls(vector<string> remoteImgUrls);

protected:
    string cascadeFilePath;
    string targetDir;
    Ptr<CascadeClassifier> cascade;
    Ptr<FaceRecognizer> modelRecognizer;
    EventDispatcher<int, void(ArmFaceIdentify::BaseEvent *event)> *eventDispatcher = nullptr;
    ArmFaceIdentify::DialogVideoFaceTrain *faceTrainHandler;
};

#endif // ARM_FACE_IDENTIFY_TRAIN_APP_H
