//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_IDENTIFYAPPLICATION_H
#define ARM_FACE_IDENTIFY_IDENTIFYAPPLICATION_H

#include "Define.h"
#include "../app_framework/Application.h"
#include "./Face/Identify.hpp"

class IdentifyApplication : public Application{
public:
    explicit IdentifyApplication(Config config);
    ~IdentifyApplication();

protected:
    void registerSubscribe();
    void beforeStart() override;
    void afterStart() override;
    Identify *getFaceIdentifyHandler();

protected:
    Identify *identify = nullptr;
};

#endif //ARM_FACE_IDENTIFY_IDENTIFYAPPLICATION_H
