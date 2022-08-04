//
// Created by rxwyun on 2022/7/13.
//

#ifndef ARM_FACE_IDENTIFY_IDENTIFY_APP_H
#define ARM_FACE_IDENTIFY_IDENTIFY_APP_H

#include <sstream>
#include "vector"
#include "opencv2/face.hpp"
#include "opencv2/opencv.hpp"
#include "../../identify_core/identify/Include/DialogVideoFaceIdentify.h"

using namespace cv;
using namespace cv::face;
using namespace std;

class Identify
{
public:
    Identify(const string& cascadeFilePath, vector<string> modelFilesPath);
    ~Identify();

    Ptr<CascadeClassifier> getCascadeClassifier();
    Ptr<FaceRecognizer> getModelRecognizer();
    ArmFaceIdentify::DialogVideoFaceIdentify *getFaceIdentifyHandler();
    void setPredictMatConfidence(double predictMatConfidence);
    vector<ArmFaceIdentify::PredictMat> startIdentifyFromVideoCapture(VideoCapture *vc);

protected:
    string cascadeFilePath;
    vector<string> modelFilesPath;
    Ptr<CascadeClassifier> cascade;
    Ptr<FaceRecognizer> modelRecognizer;
    EventDispatcher<int, void(ArmFaceIdentify::BaseEvent *event)> *eventDispatcher = nullptr;
    ArmFaceIdentify::DialogVideoFaceIdentify *faceIdentifyHandler;
    double predictMatConfidence = 2000;
};

#endif // ARM_FACE_IDENTIFY_IDENTIFY_H
