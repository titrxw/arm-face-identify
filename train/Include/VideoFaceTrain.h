//
// Created by rxwyun on 2021/8/24.
//

#ifndef ARM_FACE_IDENTIFY_VIDEOFACETRAIN_H
#define ARM_FACE_IDENTIFY_VIDEOFACETRAIN_H

#include "./FaceTrain.h"


namespace ArmFaceIdentify {
    class VideoFaceTrain : public FaceTrain {
    public:
        void trainFromVideoCapture(VideoCapture *vc, Ptr<CascadeClassifier> cascade, unsigned int label, string& targetFile);
    };
}



#endif //ARM_FACE_IDENTIFY_VIDEOFACETRAIN_H
