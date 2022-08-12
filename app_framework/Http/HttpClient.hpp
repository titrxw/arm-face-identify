//
// Created by rxwyun on 2022/8/7.
//

#ifndef ARM_FACE_IDENTIFY_HTTPCLIENT_HPP
#define ARM_FACE_IDENTIFY_HTTPCLIENT_HPP

#include <utility>

#include "sstream"
#include "map"
#include "exception"
#include <regex>
#include "curl/include/curl.h"
#include "../Util/Encrypt.hpp"
#include "../Util/Util.h"
#include "../Util/Filesystem.hpp"
#include <nlohmann/json.hpp>

using namespace std;

class HttpClient {
protected:
    string appId;
    string appSecret;

public:
    explicit HttpClient() {
    }

    ~HttpClient() {
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
        time_t curTime = time(nullptr);
        params.emplace("appid", this->appId);
        params.emplace("timestamp", to_string(curTime));
        params.emplace("nonce", Util::randomStr(16));
        params.emplace("sign", this->getSign(params, this->appSecret));

        return params;
    }

    inline std::string appendQueryParams(const std::string &path,
                                           map<string, string> params) {
        std::string path_with_query = path;
        const static std::regex re("[^?]+\\?.*");
        auto delm = std::regex_match(path, re) ? '&' : '?';
        path_with_query += delm + this->paramsToQueryStr(params);
        return path_with_query;
    }

    inline std::string paramsToQueryStr(map<string, string> params) {
        std::string query;

        for (auto it = params.begin(); it != params.end(); ++it) {
            if (it != params.begin()) { query += "&"; }
            query += it->first;
            query += "=";
            query += this->encodeQueryParam(it->second);
        }
        return query;
    }

    inline std::string encodeQueryParam(const std::string &value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (auto c : value) {
            if (std::isalnum(static_cast<uint8_t>(c)) || c == '-' || c == '_' ||
            c == '.' || c == '!' || c == '~' || c == '*' || c == '\'' || c == '(' ||
            c == ')') {
                escaped << c;
            } else {
                escaped << std::uppercase;
                escaped << '%' << std::setw(2)
                << static_cast<int>(static_cast<unsigned char>(c));
                escaped << std::nouppercase;
            }
        }

        return escaped.str();
    }

    nlohmann::json post(const string& url, map<string, string> params, map<string, string> headers = {}, int connectTimeout = 10, int timeout = 10, bool withSign = true) {
        if (withSign) {
            params = this->replenishSign(params);
        }

        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            string strResponse;

            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connectTimeout);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
            curl_easy_setopt(curl, CURLOPT_HEADER, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);

            struct curl_slist *curlHeaders = nullptr;
            curlHeaders = curl_slist_append(curlHeaders, "Content-Type: application/x-www-form-urlencoded");
            if (!headers.empty()) {
                map<string, string>::iterator iter;
                iter = headers.begin();
                while (iter != headers.end()) {
                    curlHeaders = curl_slist_append(curlHeaders, (iter->first + " " + iter->second).c_str());
                    iter++;
                }
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlHeaders);
            }
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, this->paramsToQueryStr(params).c_str());

            res = curl_easy_perform(curl);
            curl_slist_free_all(curlHeaders);
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                throw std::logic_error(curl_easy_strerror(res));
            }

            return this->parseResponse(strResponse);
        }

        throw std::logic_error(curl_easy_strerror(CURLE_FAILED_INIT));
    }

    nlohmann::json get(const string& url, map<string, string> params = {}, map<string, string> headers = {}, int connectTimeout = 10, int timeout = 10, bool withSign = true) {
        if (withSign) {
            params = this->replenishSign(params);
        }

        string queryUrl = url;
        if (!params.empty()) {
            queryUrl = this->appendQueryParams(queryUrl, params);
        }

        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            string strResponse;

            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
            curl_easy_setopt(curl, CURLOPT_URL, queryUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connectTimeout);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
            curl_easy_setopt(curl, CURLOPT_HEADER, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);

            struct curl_slist *curlHeaders = nullptr;
            if (!headers.empty()) {
                map<string, string>::iterator iter;
                iter = headers.begin();
                while (iter != headers.end()) {
                    curlHeaders = curl_slist_append(curlHeaders, (iter->first + " " + iter->second).c_str());
                    iter++;
                }
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlHeaders);
            }

            res = curl_easy_perform(curl);
            if (curlHeaders != nullptr) {
                curl_slist_free_all(curlHeaders);
            }
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                throw std::logic_error(curl_easy_strerror(res));
            }

            return this->parseResponse(strResponse);
        }

        throw std::logic_error(curl_easy_strerror(CURLE_FAILED_INIT));
    }

    nlohmann::json uploadFile(const string& url, const string& filePath, map<string, string> params = {}, map<string, string> headers = {}, int connectTimeout = 10, int timeout = 10, bool withSign = true) {
        if (withSign) {
            params = this->replenishSign(params);
        }

        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            string strResponse;

            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connectTimeout);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
            curl_easy_setopt(curl, CURLOPT_HEADER, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);

            struct curl_slist *curlHeaders = nullptr;
            if (!headers.empty()) {
                map<string, string>::iterator iter;
                iter = headers.begin();
                while (iter != headers.end()) {
                    curlHeaders = curl_slist_append(curlHeaders, (iter->first + " " + iter->second).c_str());
                    iter++;
                }
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlHeaders);
            }

            struct curl_httppost *formPost = nullptr;
            struct curl_httppost *lastptr = nullptr;
            params.emplace(Filesystem::getFileNameFromPath(filePath), filePath);
            if (!params.empty()) {
                map<string, string>::iterator piter;
                piter = params.begin();
                while (piter != params.end()) {
                    curl_formadd(&formPost,
                                 &lastptr,
                                 CURLFORM_COPYNAME, piter->first.c_str(),
                                 CURLFORM_COPYCONTENTS, piter->second.c_str(),
                                 CURLFORM_END);
                    piter++;
                }
            }
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, formPost);

            res = curl_easy_perform(curl);
            if (curlHeaders != nullptr) {
                curl_slist_free_all(curlHeaders);
            }
            curl_formfree(formPost);
            curl_formfree(lastptr);
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                throw std::logic_error(curl_easy_strerror(res));
            }

            return this->parseResponse(strResponse);
        }

        throw std::logic_error(curl_easy_strerror(CURLE_FAILED_INIT));
    }

    bool static downloadFile(const string& remoteUrl, const string& localSavePath, int connectTimeout = 10, int timeout = 10) {
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            string strResponse;

            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
            curl_easy_setopt(curl, CURLOPT_URL, remoteUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connectTimeout);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
            curl_easy_setopt(curl, CURLOPT_HEADER, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);

            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                throw std::logic_error(curl_easy_strerror(res));
            }

            Filesystem::write(localSavePath, strResponse);

            return true;
        }

        throw std::logic_error(curl_easy_strerror(CURLE_FAILED_INIT));
    }

protected:
    nlohmann::json parseResponse(string strResponse) {
        nlohmann::json jsonObj = nlohmann::json::parse(strResponse);
        if (!jsonObj.contains("data") || !jsonObj.contains("code") ||
            !jsonObj.contains("msg")) {
            throw nlohmann::json::other_error::create(400,
                                                      "JSON message missing `data`, `code`, and/or `msg` fields", nullptr);
        }

        if (jsonObj.at("code").get<std::int64_t>() != 200) {
            throw std::logic_error(jsonObj.at("msg").get<std::string>());
        }

        return jsonObj.at("data");
    }
};

#endif // ARM_FACE_IDENTIFY_HTTPCLIENT_HPP
