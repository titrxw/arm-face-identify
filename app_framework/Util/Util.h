//
// Created by rxw on 2022/8/9.
//

#ifndef ARM_FACE_IDENTIFY_UTIL_H
#define ARM_FACE_IDENTIFY_UTIL_H

#include <random>

static constexpr char CCH[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

class Util {
public:

    string static randomStr(int sz) {
        string ret;
        ret.resize(sz);
        std::mt19937 rng(std::random_device{}());
        for (int i = 0; i < sz; ++i){
            uint32_t x = rng() % (sizeof(CCH) - 1);
            ret[i] = CCH[x];
        }

        return ret;
    }
};

#endif //ARM_FACE_IDENTIFY_UTIL_H
