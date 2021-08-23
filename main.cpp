#include "train/Include/FaceTrain.h"
#include "identify/Include/FaceIdentify.h"
#include <unistd.h>
#include "helper/Include/Str.h"
#include <ctime>

using namespace ArmFaceIdentify;

string trainFace()
{
    char *tmpCurPwd = nullptr;
    tmpCurPwd = getcwd(nullptr, 0);
    string curPwd(tmpCurPwd);
    curPwd = curPwd.append("/../test/");

    string sourceFile(curPwd);
    string targetFile(curPwd);
    sourceFile = sourceFile.append("at.txt");
    targetFile = targetFile.append(Str::lToString(time(nullptr))).append("_face_model.xml");

    FaceTrain *faceTrain = new FaceTrain();
    faceTrain->trainAndSave(sourceFile, targetFile);

    delete[] tmpCurPwd;
    tmpCurPwd = nullptr;
    delete faceTrain;
    faceTrain = nullptr;

    return targetFile;
}

identifyFace(const string &modelFile)
{
    CascadeClassifier cascade;
    //训练好的文件名称，放置在可执行文件同目录下
    cascade.load(modelFile); //感觉用lbpcascade_frontalface效果没有它好，注意哈！要是正脸

    modelRecognizer = EigenFaceRecognizer::create();
    //1.加载训练好的分类器
    modelRecognizer->load(modelFile); // opencv2用load

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
}

int main()
{
    string targetFile = trainFace();
    identifyFace(targetFile);
    return 0;
}