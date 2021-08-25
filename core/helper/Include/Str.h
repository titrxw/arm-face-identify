//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_STR_H
#define ARM_FACE_IDENTIFY_STR_H

#include <sstream>

using namespace std;

namespace ArmFaceIdentify {
    class Str {
    public:
        static string lToString(long T1);
        static string lToString(unsigned int T1);
    };
}

#endif //ARM_FACE_IDENTIFY_STR_H
