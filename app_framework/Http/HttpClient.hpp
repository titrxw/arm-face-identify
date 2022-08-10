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
#include "HttpUrl.hpp"
#include "../Util/Encrypt.hpp"
#include "../Util/Util.h"
#include "../Util/Filesystem.hpp"
#include <nlohmann/json.hpp>

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

    nlohmann::json post(const string& path, map<string, string> params, map<string, string> headers = {}, bool withSign = true) {
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

    nlohmann::json get(const string& path, map<string, string> params, map<string, string> headers = {}, bool withSign = true) {
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

    nlohmann::json uploadFile(const string& path, const string& filePath, map<string, string> params = {}, map<string, string> headers = {}, bool withSign = true) {
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

    bool downloadFile(const string& remoteUrl, const string& localSavePath) {
        HttpUrl httpUrl;

        if(!httpUrl.parse(remoteUrl.c_str())){
            return false;
        }
        string host = httpUrl.get_proto();
        host += "://";
        host += httpUrl.get_domain();
        host = host.substr(0, host.length() - 1);

        string path = httpUrl.get_url_path();
        if (!to_string(httpUrl.get_url_params()).empty()) {
            path += "?";
            path += httpUrl.get_url_params();
        }

        httplib::Client client(host);
        auto res = client.Get(path);
        if (res) {
            Filesystem::write(localSavePath, res->body);

            return true;
        }

        throw std::logic_error(to_string(res.error()));
    }

protected:
    nlohmann::json parseResponse(httplib::Result res) {
        if (res) {
            nlohmann::json jsonObj = nlohmann::json::parse(res->body);
            if (!jsonObj.contains("data") || !jsonObj.contains("code") ||
                !jsonObj.contains("msg")) {
                throw nlohmann::json::other_error::create(400,
                                                          "JSON message missing `data`, `code`, and/or `msg` fields", nullptr);
            }

            if (jsonObj.at("code").get<std::int64_t>() != 200) {
                throw std::logic_error(jsonObj.at("msg").get<std::string>());
            }

            return jsonObj.at("data");

        } else {
            throw std::logic_error(to_string(res.error()));
        }
    }
};

#endif // ARM_FACE_IDENTIFY_HTTPCLIENT_HPP
