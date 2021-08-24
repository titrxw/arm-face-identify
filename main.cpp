#include "train/Include/FaceTrain.h"
#include "identify/Include/FaceIdentify.h"
#include <unistd.h>
#include "helper/Include/Str.h"

using namespace ArmFaceIdentify;

void trainFace()
{
    char *tmpCurPwd = nullptr;
    tmpCurPwd = getcwd(nullptr, 0);
    string curPwd(tmpCurPwd);
    curPwd = curPwd.append("/../test/");

    string sourceFile(curPwd);
    string targetFile(curPwd);
    sourceFile = sourceFile.append("at.txt");
    targetFile = targetFile.append("_face_model.xml");

    FaceTrain *faceTrain = new FaceTrain();
    faceTrain->trainAndSave(sourceFile, targetFile);

    delete[] tmpCurPwd;
    tmpCurPwd = nullptr;
    delete faceTrain;
    faceTrain = nullptr;
}

void identifyFace()
{
    char *tmpCurPwd = nullptr;
    tmpCurPwd = getcwd(nullptr, 0);

    string curPwd(tmpCurPwd);
    curPwd = curPwd.append("/../test/").append("haarcascade_frontalface_alt2.xml");
    Ptr<CascadeClassifier> cascade(CascadeClassifier(curPwd));

    string modelFile1(tmpCurPwd);
    modelFile1 = modelFile1.append("/../test/").append("_face_model.xml");
    Ptr<FaceRecognizer> modelRecognizer = EigenFaceRecognizer::create();
//    //1.加载训练好的分类器
    modelRecognizer->read(modelFile1); // opencv2用load

    FaceIdentify *faceIdentify = new FaceIdentify(cascade, modelRecognizer);

    VideoCapture capture(0);
    Mat frame;
    while (capture.read(frame)) //读取帧
    {
        if (frame.empty())
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }

        faceIdentify->identify(frame);

        if (waitKey(10) == 'k')
        {
            break;
        }
    }

    capture.release();
    delete faceIdentify;
}

int main()
{
    identifyFace();
    return 0;
}