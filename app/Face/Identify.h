//
// Created by rxwyun on 2022/7/13.
//

#ifndef ARM_FACE_IDENTIFY_IDENTIFY_H
#define ARM_FACE_IDENTIFY_IDENTIFY_H

#include <sstream>
#include "vector"
#include "opencv2/face.hpp"
#include "opencv2/video.hpp"

class Identify
{
public:
    Identify(string cascadeFilePath, vector<string> modelFilesPath);
    ~Identify();

    Ptr<CascadeClassifier> getCascadeClassifier();
    Ptr<FaceRecognizer> getModelRecognizer();

    void startIdentifyFromVideoCapture(VideoCapture *vc);

protected:
    string cascadeFilePath;
    string modelFilesPath;
    Ptr<CascadeClassifier> cascade;
    Ptr<FaceRecognizer> modelRecognizer;
};

#endif // ARM_FACE_IDENTIFY_IDENTIFY_H
