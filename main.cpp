#include "core/train/Include/FaceTrain.h"
#include "business/train/Include/DialogVideoFaceTrain.h"
#include "business/identify/Include/DialogVideoFaceIdentify.h"
#include <unistd.h>
#include "core/helper/Include/Str.h"

using namespace ArmFaceIdentify;

void trainFaceFromVideo()
{
    char *tmpCurPwd = nullptr;
    tmpCurPwd = getcwd(nullptr, 0);
    string curPwd(tmpCurPwd);

    string cascaedFile(getenv("ARM_DACE_IDENTIFY_DATA_PATH"));
    cascaedFile = cascaedFile.append("haarcascade_frontalface_alt.xml");
    cout <<cascaedFile<<endl;
    Ptr<CascadeClassifier> cascade(new CascadeClassifier(cascaedFile));

    string targetFile(curPwd);
    targetFile = targetFile.append("/test/lyr/");
    Ptr<FaceRecognizer> modelRecognizer = EigenFaceRecognizer::create();

    VideoCapture capture(0);

    DialogVideoFaceTrain *faceTrain = new DialogVideoFaceTrain(cascade, modelRecognizer, &capture, targetFile);
    faceTrain->train(18);

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

    string curPwd("");
    curPwd = curPwd.append("/use/local/arm-face-identify/data/").append("haarcascade_frontalface_alt.xml");
    Ptr<CascadeClassifier> cascade(new CascadeClassifier(curPwd));

    string modelFile1(tmpCurPwd);
    modelFile1 = modelFile1.append("/../test/rxw/").append("face_model.xml");
    Ptr<FaceRecognizer> modelRecognizer = EigenFaceRecognizer::create();
//    //1.加载训练好的分类器
    modelRecognizer->read(modelFile1); // opencv2用load

    VideoCapture capture(0);
    DialogVideoFaceIdentify *faceIdentify = new DialogVideoFaceIdentify(cascade, modelRecognizer, &capture);
    faceIdentify->identifyFromVideo();

//    cascade.release();
    delete faceIdentify;
    faceIdentify = nullptr;
}

int main()
{
    trainFaceFromVideo();
//    trainFace();
//    identifyFace();
    return 0;
}