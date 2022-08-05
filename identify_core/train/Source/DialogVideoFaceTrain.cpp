//
// Created by rxwyun on 2021/8/24.
//

#include "../../helper/Include/Str.h"
#include "../../base/Enum/Event.h"
#include "../../helper/Include/File.h"
#include "../Include/DialogVideoFaceTrain.h"

ArmFaceIdentify::DialogVideoFaceTrain::DialogVideoFaceTrain(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer,
                                                            EventDispatcher<int, void(ArmFaceIdentify::BaseEvent *)> *eventDispatcher,
                                                            const string &targetDir) : targetDir(targetDir), ArmFaceIdentify::FaceTrain(cascade, modelRecognizer, eventDispatcher)
{
    this->getEventDispatcher()->appendListener(ArmFaceIdentify::Event::DETECTED_FEATURE_IMAGE_FROM_FRAME, [this](ArmFaceIdentify::BaseEvent *event)
                                               { this->onDetectedFaceListener((ArmFaceIdentify::DetectedFeatureMatEvent *)event); });
}

void ArmFaceIdentify::DialogVideoFaceTrain::onDetectedFaceListener(ArmFaceIdentify::DetectedFeatureMatEvent *event)
{
    rectangle(event->detectedFace.sourceMat, Point(event->detectedFace.mat.x, event->detectedFace.mat.y), Point(event->detectedFace.mat.x + event->detectedFace.mat.width, event->detectedFace.mat.y + event->detectedFace.mat.height),
              Scalar(0, 255, 0), 1, 8);

    string label = string("第").append(event->options["cur_num"].append("张, 共").append(ArmFaceIdentify::Str::toString(DialogVideoFaceTrain::DETECTED_FACE_NUM)).append("张"));
    putText(event->detectedFace.sourceMat, label, event->detectedFace.mat.tl(), FONT_HERSHEY_COMPLEX, 1.2, (0, 0, 255), 2, 0);
}

void ArmFaceIdentify::DialogVideoFaceTrain::setCanDetectedNextMatWithFlag(string flag){
    this->canDetectedNextMatFlag = flag;
}

bool ArmFaceIdentify::DialogVideoFaceTrain::isCanDetectedNextMat() const {
    return waitKey(10) == 'n' || !this->canDetectedNextMatFlag.empty();
}

void ArmFaceIdentify::DialogVideoFaceTrain::stopDetectedFromVideo()
{
    this->stopDetectedMat = true;
}

bool ArmFaceIdentify::DialogVideoFaceTrain::ifNecessaryStop() const
{
    return waitKey(10) == 'k' || this->stopDetectedMat;
}

string ArmFaceIdentify::DialogVideoFaceTrain::makeSampleFileFromVideoCapture(VideoCapture *vc, unsigned int label)
{
    Mat frame;
    unsigned int picNum = 1;
    string modeFileContent;

    string imgDir(this->targetDir);
    imgDir = imgDir.append("img/").append(ArmFaceIdentify::Str::toString(label)).append("/");
    ArmFaceIdentify::File::mkdirs(imgDir);
    ArmFaceIdentify::File::removeDir(imgDir);

    while (vc->read(frame))
    {
        if (frame.empty())
        {
            throw "video capture read frame empty";
        }

        imshow(DialogVideoFaceTrain::DIALOG_NAME, frame);

        if (this->isCanDetectedNextMat())
        {
            this->setCanDetectedNextMatWithFlag("");

            vector<ArmFaceIdentify::DetectedMat> detectedFaceMap = this->detectFaceMatFromMat(this->cascade, frame);
            if (detectedFaceMap.size() == 1)
            {
                string matFileName(imgDir);
                matFileName = matFileName.append(format("%d.jpg", picNum)); //存放在当前项目文件夹以1-10.jpg 命名，format就是转为字符串
                imwrite(matFileName, detectedFaceMap[0].detectMat);

                modeFileContent = modeFileContent.append(matFileName).append(";").append(ArmFaceIdentify::Str::toString(label)).append("\n");
                ++picNum;
            }
            detectedFaceMap.clear();
        }

        if (this->ifNecessaryStop())
        {
            this->stopDetectedMat = false;
            break;
        }
    }
    cvDestroyWindow(DialogVideoFaceTrain::DIALOG_NAME.c_str());

    string faceSampleFile(this->targetDir);
    faceSampleFile = faceSampleFile.append(ArmFaceIdentify::Str::toString(label)).append("_").append(DialogVideoFaceTrain::SAMPLE_FILE_NAME);
    ArmFaceIdentify::File::write(faceSampleFile, modeFileContent);

    return faceSampleFile;
}

string ArmFaceIdentify::DialogVideoFaceTrain::trainFromVideoCapture(VideoCapture *vc, unsigned int label)
{
    //文件按照label_filename的方式命名, 训练的时候找到所有类似的文件,生成临时文件,训练完成后删除
    string sampleFilePath = this->makeSampleFileFromVideoCapture(vc, label);

    string sampleTrainFile(this->targetDir);
    try
    {
        sampleTrainFile = sampleTrainFile.append(DialogVideoFaceTrain::SAMPLE_FILE_TRAIN_NAME);
        this->trainAndSave(sampleFilePath, sampleTrainFile);
        ArmFaceIdentify::File::unlink(sampleFilePath);
    }
    catch (std::exception e)
    {
        ArmFaceIdentify::File::unlink(sampleFilePath);
        throw e;
    }

    return sampleTrainFile;
}

ArmFaceIdentify::DialogVideoFaceTrain::~DialogVideoFaceTrain()
{
}
