//
// Created by rxwyun on 2021/8/24.
//

#ifndef ARM_FACE_IDENTIFY_DIALOGVIDEOFACETRAIN_H
#define ARM_FACE_IDENTIFY_DIALOGVIDEOFACETRAIN_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "FaceTrain.h"
#include "../../base/Include/Event/DetectedFeatureMatEvent.h"

namespace ArmFaceIdentify
{
    class DialogVideoFaceTrain : public ArmFaceIdentify::FaceTrain
    {
    public:
        DialogVideoFaceTrain(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer,
                             EventDispatcher<int, void(ArmFaceIdentify::BaseEvent *event)> *eventDispatcher,
                             const string &targetDir);

        ~DialogVideoFaceTrain();

        const string DIALOG_NAME = "arm_face_identify_dialog";

        string trainFromVideoCapture(VideoCapture *vc, unsigned int label);
        void setCanDetectedNextMat(bool can);
        void stopDetectedFromVideo();
        bool ifNecessaryStop() const;

    protected:
        const string &targetDir;
        bool canDetectedNextMat = false;
        bool stopDetectedMat = false;

        const int DETECTED_FACE_NUM = 30;
        const string SAMPLE_FILE_NAME = "arm_face_sample.txt";
        const string SAMPLE_FILE_TRAIN_NAME = "arm_face_sample_train.xml";

        string makeSampleFileFromVideoCapture(VideoCapture *vc, unsigned int label);

        void onDetectedFaceListener(ArmFaceIdentify::DetectedFeatureMatEvent *event);
    };
}

#endif // ARM_FACE_IDENTIFY_DIALOGVIDEOFACETRAIN_H
