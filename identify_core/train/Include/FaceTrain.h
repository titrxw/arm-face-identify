//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_FACETRAIN_H
#define ARM_FACE_IDENTIFY_FACETRAIN_H

#include "Train.h"

namespace ArmFaceIdentify
{
    class FaceTrain : public Train
    {
    public:
        FaceTrain(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer, EventDispatcher<int, void(ArmFaceIdentify::BaseEvent *event)> *eventDispatcher = nullptr);
        ~FaceTrain();

    protected:
        Ptr<CascadeClassifier> cascade;
        Ptr<FaceRecognizer> modelRecognizer;

        void loadSourceFile(const string &filename, vector<Mat> &mats, vector<int> &matLabels, char separator = ';');
        void trainMats(vector<Mat> &mats, vector<int> &matLabels, const string &targetFile);
    };
}

#endif // ARM_FACE_IDENTIFY_FACETRAIN_H
