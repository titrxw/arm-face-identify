//
// Created by rxw on 2022/8/3.
//

#ifndef ARM_FACE_IDENTIFY_CONFIG_APP_H
#define ARM_FACE_IDENTIFY_CONFIG_APP_H

#include <sstream>
#include "configor/configor.hpp"
#include "configor/json.hpp"

using namespace std;
using namespace configor;

class App {
public:
    string appName;

    CONFIGOR_BIND(json, App, REQUIRED(appName, "app_name"));
};

#endif //ARM_FACE_IDENTIFY_CONFIG_APP_H
