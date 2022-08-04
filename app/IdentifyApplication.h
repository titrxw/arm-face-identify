//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_IDENTIFYAPPLICATION_H
#define ARM_FACE_IDENTIFY_IDENTIFYAPPLICATION_H

#include "../app_framework/Application.h"

class IdentifyApplication : public Application{
public:
    explicit IdentifyApplication(Config config);
    ~IdentifyApplication();

protected:
    void registerSubscribe();
    void beforeStart();
    void afterStart();
};


#endif //ARM_FACE_IDENTIFY_IDENTIFYAPPLICATION_H
