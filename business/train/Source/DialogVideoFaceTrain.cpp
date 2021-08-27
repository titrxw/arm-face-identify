//
// Created by rxwyun on 2021/8/24.
//

#include "../../../core/helper/Include/Str.h"
#include "../../../core/helper/Include/File.h"
#include "../Include/DialogVideoFaceTrain.h"

void DialogVideoFaceTrain::onDetectedFace(ArmFaceIdentify::DetectedFace detectedFace, unsigned int curNum) {
    rectangle(detectedFace.sourceMat, Point(detectedFace.face.x, detectedFace.face.y), Point(detectedFace.face.x + detectedFace.face.width, detectedFace.face.y + detectedFace.face.height),
              Scalar(0, 255, 0), 1, 8);

    string label = string("第").append(ArmFaceIdentify::Str::toString(curNum).append("张, 共").append(ArmFaceIdentify::Str::toString(DialogVideoFaceTrain::DETECTED_FACE_NUM)).append("张"));
    putText(detectedFace.sourceMat, label, detectedFace.face.tl(), FONT_HERSHEY_COMPLEX, 1.2,  (0, 0, 255), 2, 0);
}

string DialogVideoFaceTrain::makeSampleFile(unsigned int label) {
    Mat frame;
    unsigned int picNum = 1;
    string modeFileContent;
    string dialogName = "arm_face_identify_dialog";

    string imgDir(this->targetDir);
    imgDir = imgDir.append("img/").append(ArmFaceIdentify::Str::toString(label)).append("/");
    ArmFaceIdentify::File::mkdirs(imgDir);

    while (this->vc->read(frame)) {
        if (frame.empty()) {
            break;
        }

        vector<ArmFaceIdentify::DetectedFace> detectedFaceMap = this->detectFaceMatFromMat(this->cascade, frame);
        if (detectedFaceMap.size() == 1) {
            string matFileName(this->targetDir);
            matFileName = matFileName.append(format("%d.jpg", picNum)); //存放在当前项目文件夹以1-10.jpg 命名，format就是转为字符串
            imwrite(matFileName, detectedFaceMap[0].detectMat);

            this->onDetectedFace(detectedFaceMap[0], picNum);

            modeFileContent = modeFileContent.append(matFileName).append(";").append(ArmFaceIdentify::Str::toString(label)).append("\n");

            ++picNum;
            waitKey(500);
        }
        detectedFaceMap.clear();

        imshow(dialogName, frame);

        if (picNum == 30) {
            break;
        }
    }
    cvDestroyWindow(dialogName.c_str());

    string faceSampleFile(this->targetDir);
    faceSampleFile = faceSampleFile.append(ArmFaceIdentify::Str::toString(label)).append("_").append(DialogVideoFaceTrain::SAMPLE_FILE_NAME);
    ArmFaceIdentify::File::write(faceSampleFile, modeFileContent);

    return faceSampleFile;
}

void DialogVideoFaceTrain::train(unsigned int label) {
    //文件按照label_filename的方式命名, 训练的时候找到所有类似的文件,生成临时文件,训练完成后删除
    string sampleFilePath = this->makeSampleFile(label);
    //预训练,保证数据正确
    try {
        this->trainAndSave(sampleFilePath);
    } catch (exception e) {
        ArmFaceIdentify::File::unlink(sampleFilePath);
        throw e;
    }

    string sampleContent;
    vector<string> files = ArmFaceIdentify::File::glob(string(this->targetDir).append("*_").append(DialogVideoFaceTrain::SAMPLE_FILE_NAME));
    for(int i = 0; i < files.size(); i++) {
        sampleContent = sampleContent.append(ArmFaceIdentify::File::read(files[i]));
    }
    string tmpSampleFile(this->targetDir);
    tmpSampleFile = tmpSampleFile.append(DialogVideoFaceTrain::SAMPLE_FILE_NAME);
    ArmFaceIdentify::File::write(tmpSampleFile, sampleContent);

    try {
        string sampleTrainFile(this->targetDir);
        sampleTrainFile = sampleTrainFile.append(DialogVideoFaceTrain::SAMPLE_FILE_TRAIN_NAME);
        this->trainAndSave(tmpSampleFile, sampleTrainFile);
        ArmFaceIdentify::File::unlink(tmpSampleFile.c_str());
    } catch (std::exception e) {
        ArmFaceIdentify::File::unlink(tmpSampleFile.c_str());
        throw e;
    }
}

DialogVideoFaceTrain::~DialogVideoFaceTrain() {
    if (!this->cascade.empty()) {
        this->cascade.release();
    }
    if (this->vc) {
        this->vc->release();
        this->vc = nullptr;
    }
}
