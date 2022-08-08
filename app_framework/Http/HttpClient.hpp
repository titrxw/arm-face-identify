//
// Created by rxwyun on 2022/8/7.
//

#ifndef ARM_FACE_IDENTIFY_HTTPCLIENT_HPP
#define ARM_FACE_IDENTIFY_HTTPCLIENT_HPP

#include "sstream"
#include "map"
#include "httplib/httplib.h"
#include "../Util/Encrypt.hpp"

using namespace std;

class HttpClient {
public:
    httplib::Client static getClient(const string& serverAddress) {
        httplib::Client client(serverAddress);
        client.set_compress(true);
        client.set_decompress(true);

        return client;
    }

    string static getSign(map<string, string> params, const string& secret) {
        vector<string> keys;
        map<string, string>::iterator iter;
        iter = params.begin();
        while(iter != params.end()) {
            keys.push_back(iter->first);
            iter++;
        }
        sort(keys.begin(), keys.end());
        string url;
        int size_t = keys.size();
        for (int i = 0; i < size_t; ++i) {
            url += keys[i] + "=" + params[keys[i]];
            if (i != size_t - 1) {
                url += "&";
            }
        }

        return Encrypt::md5(url + secret);
    }
};

#endif // ARM_FACE_IDENTIFY_HTTPCLIENT_HPP
