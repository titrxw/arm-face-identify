//
// Created by rxwyun on 2021/8/22.
//

#ifndef ARM_FACE_IDENTIFY_STR_H
#define ARM_FACE_IDENTIFY_STR_H

#include <sstream>

using namespace std;

namespace ArmFaceIdentify
{
    class Str
    {
    public:
        static string toString(long T1);
        static string toString(unsigned int T1);
        static string toString(int T1);
        static string toString(double T1);
    };
}

#endif // ARM_FACE_IDENTIFY_STR_H
