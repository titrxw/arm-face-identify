//
// Created by rxwyun on 2021/8/22.
//

#include "../Include/Str.h"

string ArmFaceIdentify::Str::lToString(long T1) {
    ostringstream oss;  //创建一个格式化输出流
    oss<<T1;             //把值传递如流中
    return oss.str();
}

string ArmFaceIdentify::Str::lToString(unsigned int T1) {
    ostringstream oss;  //创建一个格式化输出流
    oss<<T1;             //把值传递如流中
    return oss.str();
}
