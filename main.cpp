#include "core/train/Include/FaceTrain.h"
#include "core/train/Include/DialogVideoFaceTrain.h"
#include "core/identify/Include/DialogVideoFaceIdentify.h"
#include <unistd.h>
#include <cstdlib>
#include "core/helper/Include/Str.h"
#include "asio/asio.hpp"

using namespace ArmFaceIdentify;

void trainFaceFromVideo()
{
    char *tmpCurPwd = nullptr;
    tmpCurPwd = getcwd(nullptr, 0);
    string curPwd(tmpCurPwd);

    //env OPENCV_SAMPLES_DATA_PATH OPENCV_DATA_PATH
    //从env中获取设置好的OPENCV_SAMPLES_DATA_PATH 或者OPENCV_DATA_PATH
    string cascaedFile = samples::findFile("haarcascade_frontalface_alt.xml").c_str();
    Ptr<CascadeClassifier> cascade(new CascadeClassifier(cascaedFile));

    string targetFile(curPwd);
    targetFile = targetFile.append("/test/");
    Ptr<FaceRecognizer> modelRecognizer = EigenFaceRecognizer::create();

    VideoCapture capture(0);

    DialogVideoFaceTrain *faceTrain = new DialogVideoFaceTrain(cascade, modelRecognizer, new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)>(), targetFile);
    faceTrain->trainFromVideoCapture(&capture, 18);

    delete[] tmpCurPwd;
    tmpCurPwd = nullptr;
    delete faceTrain;
    faceTrain = nullptr;
    capture.release();
    cascade.release();
}

void identifyFace()
{
    char *tmpCurPwd = nullptr;
    tmpCurPwd = getcwd(nullptr, 0);

//    string cascaedFile = "/usr/local/arm-face-identify/data/haarcascade_frontalface_alt.xml";
    string cascaedFile = samples::findFile("haarcascade_frontalface_alt.xml").c_str();
//    samples::findFile("haarcascade_frontalface_alt.xml").c_str();
    Ptr<CascadeClassifier> cascade(new CascadeClassifier(cascaedFile));

    string modelFile1(tmpCurPwd);
    modelFile1 = modelFile1.append("/../test/rxw/").append("face_model.xml");
    Ptr<FaceRecognizer> modelRecognizer = EigenFaceRecognizer::create();
//    //1.加载训练好的分类器
    modelRecognizer->read(modelFile1); // opencv2用load

    VideoCapture capture(0);
    DialogVideoFaceIdentify *faceIdentify = new DialogVideoFaceIdentify(cascade, modelRecognizer, new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)>());
    faceIdentify->identifyFromVideoCapture(&capture);

//    cascade.release();
    delete faceIdentify;
    faceIdentify = nullptr;
}

int main()
{
//    trainFaceFromVideo();
//    trainFace();
    identifyFace();
    return 0;
}