//
// Created by rxwyun on 2021/8/24.
//

#include "../../../core/helper/Include/Str.h"
#include "../../../core/helper/Include/File.h"
#include "../Include/VideoFaceTrain.h"

string VideoFaceTrain::getSampleVideoCapture(VideoCapture *vc, Ptr<CascadeClassifier> cascade,
                                                            unsigned int label,
                                                            string &targetDir) {
    Mat frame;
    int picNum = 1;
    string modeFileContent;
    while (vc->read(frame)) {
        if (frame.empty()) {
            break;
        }

        frame = VideoFaceTrain::pretreatmentMat(frame);
        vector<Mat> pMats = this->getFaceMatFromMat(cascade, frame);
        if (pMats.size() == 1) {
            string matFileName(targetDir);
            matFileName = matFileName.append(format("%d.jpg", picNum)); //存放在当前项目文件夹以1-10.jpg 命名，format就是转为字符串
            imwrite(matFileName, pMats[0]);//存在当前目录下

            modeFileContent = modeFileContent.append(matFileName).append(";").append(ArmFaceIdentify::Str::lToString(label)).append("\n");

            ++picNum;
            waitKey(500);
        }

        if (picNum == 30) {
            break;
        }
    }

    string faceModelTxt(targetDir);
    faceModelTxt = faceModelTxt.append("face_model.txt");
    ArmFaceIdentify::File::write(faceModelTxt, modeFileContent);

    return faceModelTxt;
}
