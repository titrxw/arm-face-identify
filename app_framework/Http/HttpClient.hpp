//
// Created by rxwyun on 2022/8/7.
//

#ifndef ARM_FACE_IDENTIFY_HTTPCLIENT_HPP
#define ARM_FACE_IDENTIFY_HTTPCLIENT_HPP

#include <utility>

#include "sstream"
#include "map"
#include "exception"
#include "httplib/httplib.h"
#include "../Util/Encrypt.hpp"
#include "../Util/Util.h"
#include "../Util/Filesystem.hpp"

using namespace std;

class HttpClient {
protected:
    string serverAddress;
    string appId;
    string appSecret;
    httplib::Client *client;

public:
    explicit HttpClient(string serverAddress) : serverAddress(std::move(serverAddress)) {
        this->client = new httplib::Client(this->serverAddress);
        this->client->set_compress(true);
        this->client->set_decompress(true);
    }

    ~HttpClient() {
        delete this->client;
        this->client = nullptr;
    }

    HttpClient* withAppId(string appId) {
        this->appId = appId;
        return this;
    }

    HttpClient* withAppSecret(string appSecret) {
        this->appSecret = appSecret;
        return this;
    }

    string getSign(map<string, string> params, const string &secret) {
        vector<string> keys;
        map<string, string>::iterator iter;
        iter = params.begin();
        while (iter != params.end())
        {
            keys.push_back(iter->first);
            iter++;
        }
        sort(keys.begin(), keys.end());
        string url;
        int size_t = keys.size();
        for (int i = 0; i < size_t; ++i)
        {
            url += keys[i] + "=" + params[keys[i]];
            if (i != size_t - 1)
            {
                url += "&";
            }
        }

        return Encrypt::md5(url + secret);
    }

    map<string, string> replenishSign(map<string, string>& params) {
        time_t curTime = time(NULL);
        params.emplace("appid", this->appId);
        params.emplace("timestamp", to_string(curTime));
        params.emplace("nonce", Util::randomStr(16));
        params.emplace("sign", this->getSign(params, this->appSecret));

        return params;
    }

    map<string, string> post(const string& path, map<string, string> params, map<string, string> headers = {}, bool withSign = true) {
        if (withSign) {
            params = this->replenishSign(params);
        }

        httplib::Params httpParams;
        map<string, string>::iterator iter;
        iter = params.begin();
        while (iter != params.end()) {
            httpParams.emplace(iter->first, iter->second);
            iter++;
        }

        httplib::Headers httpHeaders;
        iter = headers.begin();
        while (iter != headers.end()) {
            httpHeaders.emplace(iter->first, iter->second);
            iter++;
        }

        return this->parseResponse(this->client->Post(path, httpHeaders, httpParams));
    }

    map<string, string> get(const string& path, map<string, string> params, map<string, string> headers = {}, bool withSign = true) {
        if (withSign) {
            params = this->replenishSign(params);
        }

        httplib::Params httpParams;
        map<string, string>::iterator iter;
        iter = params.begin();
        while (iter != params.end()) {
            httpParams.emplace(iter->first, iter->second);
            iter++;
        }

        httplib::Headers httpHeaders;
        iter = headers.begin();
        while (iter != headers.end()) {
            httpHeaders.emplace(iter->first, iter->second);
            iter++;
        }

        return this->parseResponse(this->client->Get(path, httpParams, httpHeaders));
    }

    map<string, string> uploadFile(const string& path, string filePath, map<string, string> params = {}, map<string, string> headers = {}, bool withSign = true) {
        if (withSign) {
            params = this->replenishSign(params);
        }

        httplib::MultipartFormDataItems httpParams;
        map<string, string>::iterator iter;
        iter = params.begin();
        while (iter != params.end()) {
            httpParams.push_back({iter->first, iter->second, "", ""});
            iter++;
        }
        httpParams.push_back({"file", Filesystem::read(filePath), Filesystem::getFileNameFromPath(filePath), "application/octet-stream"});

        httplib::Headers httpHeaders;
        iter = headers.begin();
        while (iter != headers.end())
        {
            httpHeaders.emplace(iter->first, iter->second);
            iter++;
        }

        return this->parseResponse(this->client->Post(path, httpHeaders, httpParams));
    }

protected:
    map<string, string> parseResponse(httplib::Result res) {
        if (res) {
            cout << res->status << endl;
            cout << res->body << endl;
        } else {
            throw std::logic_error(to_string(res.error()));
        }

        return {};
    }
};

#endif // ARM_FACE_IDENTIFY_HTTPCLIENT_HPP
