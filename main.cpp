//#include "core/train/Include/FaceTrain.h"
//#include "core/train/Include/DialogVideoFaceTrain.h"
//#include "core/identify/Include/DialogVideoFaceIdentify.h"
//#include <unistd.h>
//#include "core/helper/Include/Str.h"
//#include "app/Face/Train.h"
#include <fstream>
#include "app/App.h"
#include "config/Config.h"
#include "config/Device.h"
#include "config/Mqtt.h"

//using namespace ArmFaceIdentify;

//void trainFaceFromVideo()
//{
//    char *tmpCurPwd = nullptr;
//    tmpCurPwd = getcwd(nullptr, 0);
//    string curPwd(tmpCurPwd);
//
//    //env OPENCV_SAMPLES_DATA_PATH OPENCV_DATA_PATH
//    //从env中获取设置好的OPENCV_SAMPLES_DATA_PATH 或者OPENCV_DATA_PATH
//    samples::addSamplesDataSearchPath(curPwd + "/../data");
//    string cascaedFile = samples::findFile("haarcascade_frontalface_alt.xml").c_str();
//    Ptr<CascadeClassifier> cascade(new CascadeClassifier(cascaedFile));
//
//    string targetFile(curPwd);
//    targetFile = targetFile.append("/test/");
//    Ptr<FaceRecognizer> modelRecognizer = EigenFaceRecognizer::create();
//
//    VideoCapture capture(0);
//
//    DialogVideoFaceTrain *faceTrain = new DialogVideoFaceTrain(cascade, modelRecognizer, new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)>(), targetFile);
//    faceTrain->trainFromVideoCapture(&capture, 18);
//
//    delete[] tmpCurPwd;
//    tmpCurPwd = nullptr;
//    delete faceTrain;
//    faceTrain = nullptr;
//    capture.release();
//    cascade.release();
//}
//
//void identifyFace()
//{
//    char *tmpCurPwd = nullptr;
//    tmpCurPwd = getcwd(nullptr, 0);
//    string curPwd(tmpCurPwd);
//
////    string cascaedFile = "/usr/local/arm-face-identify/data/haarcascade_frontalface_alt.xml";
//    samples::addSamplesDataSearchPath(curPwd + "/../data");
//    string cascaedFile = samples::findFile("haarcascade_frontalface_alt.xml").c_str();
////    samples::findFile("haarcascade_frontalface_alt.xml").c_str();
//    Ptr<CascadeClassifier> cascade(new CascadeClassifier(cascaedFile));
//
//    string modelFile1(tmpCurPwd);
//    modelFile1 = modelFile1.append("/test/").append("arm_face_sample_train.xml");
//    Ptr<FaceRecognizer> modelRecognizer = EigenFaceRecognizer::create();
////    //1.加载训练好的分类器
//    modelRecognizer->read(modelFile1); // opencv2用load
//
//    VideoCapture capture(0);
//    DialogVideoFaceIdentify *faceIdentify = new DialogVideoFaceIdentify(cascade, modelRecognizer, new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)>());
//    faceIdentify->identifyFromVideoCapture(&capture);
//
////    cascade.release();
//    delete faceIdentify;
//    faceIdentify = nullptr;
//}

int main()
{
    Config config;
    std::ifstream ifs("/Users/rxwyun/Develop/c/arm-face-identify/config.json");

    json j;
    ifs >> j;
    config = (Config)j;

    (new App(config))->start();
    while (std::tolower(std::cin.get()) != 'q')
        ;
//    new ::Train("dsf", "sdfdsf");
//    trainFaceFromVideo();
//    trainFace();
//    identifyFace();
    return 0;
}