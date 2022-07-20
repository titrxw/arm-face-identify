//
// Created by rxwyun on 2022/7/13.
//

#ifndef ARM_FACE_IDENTIFY_TRAIN_APP_H
#define ARM_FACE_IDENTIFY_TRAIN_APP_H

#include <sstream>
#include "vector"
#include "opencv2/face.hpp"

class Train
{
public:
    Train(string cascadeFilePath, string targetDir);
    ~Train();

    Ptr<CascadeClassifier> getCascadeClassifier();
    Ptr<FaceRecognizer> getModelRecognizer();

    string trainFromRemoteImgUrls(vector<string> remoteImgUrls);

protected:
    string cascadeFilePath;
    string targetDir;
    Ptr<CascadeClassifier> cascade;
    Ptr<FaceRecognizer> modelRecognizer;
};

#endif // ARM_FACE_IDENTIFY_TRAIN_APP_H
