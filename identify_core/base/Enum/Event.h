//
// Created by rxw on 2021/8/25.
//

namespace ArmFaceIdentify {
    enum Event {
        DETECTED_FEATURE_IMAGE_FROM_FRAME = 1,
        PREDICT_FEATURE_IMAGE_FROM_FRAME = 2,
        DETECTED_FEATURE_IMAGE_FROM_FRAME_COMPLETE = 3,
        PREDICT_FEATURE_IMAGE_FROM_FRAME_COMPLETE = 4
    };
}