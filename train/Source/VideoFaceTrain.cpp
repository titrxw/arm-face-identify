//
// Created by rxwyun on 2021/8/24.
//

#include "../../helper/Include/Str.h"
#include "../../helper/Include/File.h"
#include "../Include/VideoFaceTrain.h"

void ArmFaceIdentify::VideoFaceTrain::trainFromVideoCapture(VideoCapture *vc, Ptr<CascadeClassifier> cascade, unsigned int label, string& targetFile) {
    Mat frame;
    int picNum = 1;
    string modeFileContent;
    while (vc->read(frame)) {
        if (frame.empty()) {
            break;
        }

        vector<Mat> pMats = ArmFaceIdentify::VideoFaceTrain::getFaceMatFromMat(cascade, frame);
        if (pMats.size() == 1) {
            string matFileName(targetFile);
            matFileName = matFileName.append(format("%d.jpg", picNum)); //存放在当前项目文件夹以1-10.jpg 命名，format就是转为字符串
            imwrite(matFileName, pMats[0]);//存在当前目录下

            modeFileContent = modeFileContent.append(matFileName).append(";").append(Str::lToString(label)).append("\n");

            ++picNum;
            waitKey(500);
        }

        if (picNum == 30) {
            break;
        }
    }

    string faceModelTxt(targetFile);
    faceModelTxt = faceModelTxt.append("face_model.txt");
    File::write(faceModelTxt, modeFileContent);

    string faceModelXml(targetFile);
    faceModelXml = faceModelXml.append("face_model.xml");
    this->trainAndSave(faceModelTxt, faceModelXml);
}
