//
// Created by rxw on 2021/8/26.
//

#ifndef ARM_FACE_IDENTIFY_DIALOGVIDEOFACEIDENTIFY_H
#define ARM_FACE_IDENTIFY_DIALOGVIDEOFACEIDENTIFY_H

#include "FaceIdentify.h"
#include "../../base/Include/Event/DetectedFeatureMatEvent.h"
#include "../../base/Include/Event/PredictFeatureMatEvent.h"

namespace ArmFaceIdentify {
    class DialogVideoFaceIdentify : public FaceIdentify {
    public:
        DialogVideoFaceIdentify(Ptr<CascadeClassifier> cascade, Ptr<FaceRecognizer> modelRecognizer,
                                EventDispatcher<int, void(ArmFaceIdentify::BaseEvent *)> *eventDispatcher);
        ~DialogVideoFaceIdentify();

        const string DIALOG_NAME = "arm_face_identify_dialog";

        void identifyFromVideoCapture(VideoCapture *vc);
        void setCanIdentifyNextMatWithFlag(string flag);
        bool isCanIdentifyNextMat() const;
        void stopIdentifyFromVideo();
        void onDetectedFaceListener(ArmFaceIdentify::DetectedFeatureMatEvent *event);
        void onPredictFaceListener(ArmFaceIdentify::PredictFeatureMatEvent *event);
        void setPredictMatMapCallback(std::function<void (vector<ArmFaceIdentify::PredictMat>, string)> validator);

    protected:
        bool isStopIdentify = false;
        string canIdentifyNextMatFlag = "";
        std::function<void (vector<ArmFaceIdentify::PredictMat>, string)> predictMatMapCallback;
        bool ifNecessaryStop();
    };
}

#endif //ARM_FACE_IDENTIFY_DIALOGVIDEOFACEIDENTIFY_H
