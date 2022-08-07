//
// Created by rxwyun on 2022/8/7.
//

#ifndef ARM_FACE_IDENTIFY_SERVER_H
#define ARM_FACE_IDENTIFY_SERVER_H

#include <sstream>
#include "configor/configor.hpp"
#include "configor/json.hpp"

using namespace std;
using namespace configor;

class Server {
public:
    string httpServerAddress;
    CONFIGOR_BIND(json, Server, REQUIRED(httpServerAddress, "http_server_address"));
};


#endif //ARM_FACE_IDENTIFY_SERVER_H
