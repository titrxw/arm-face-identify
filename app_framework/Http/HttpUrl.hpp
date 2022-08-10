//
// Created by rxw on 2022/8/10.
//

#ifndef ARM_FACE_IDENTIFY_HTTPURL_HPP
#define ARM_FACE_IDENTIFY_HTTPURL_HPP

#define	HTTP_PREFIX	"http://"
#define	HTTPS_PREFIX	"https://"
#define SKIP_WHILE(cond, ptr) { while(*ptr && (cond)) ptr++; }

#include "string"

using namespace std;

class  HttpUrl {
public:
    HttpUrl() {
        strncpy(proto_, "http", sizeof(proto_));
        port_ = 80;
    }
    ~HttpUrl() = default;

    bool parse(const char* url)  {
        const char* ptr;

        if (!strncasecmp(url, HTTP_PREFIX, sizeof(HTTP_PREFIX) - 1)) {
            ptr = url + sizeof(HTTP_PREFIX) - 1;
        } else if (!strncasecmp(url, HTTPS_PREFIX, sizeof(HTTPS_PREFIX) - 1)) {
            ptr = url + sizeof(HTTPS_PREFIX) - 1;
            port_ = 443;
            strncpy(proto_, "https", sizeof(proto_));
        } else if (*url == '/'){
            ptr = url;
        } else {
            return false;
        }

        if (*ptr == 0) {
            return false;
        }

        if (ptr == url) {
            // 说明是仅含相对路径的 url
            return parse_url_part(url);
        } else {
            // 说明包含有完整路径的 url，下面先提取域名字段，再提取相对 url
            ptr = parse_domain(ptr);
            if (ptr == nullptr) {
                url_path_ = "/";
                return true;
            }
            return parse_url_part(ptr);
        }
    }

    /**
     * 返回 URL 中的协议类型：http 或 https
     * @return {const char*}
     */
    const char* get_proto() const {
        return proto_;
    }

    /**
     * 返回 URL 中的域名字段
     * @return {const char*} 返回空串则表示没有该字段
     */
    const char* get_domain() const {
        return domain_.c_str();
    }

    /**
     * 返回根据 URL 提取的 HTTP 协议服务端端口号，内部缺省值为 80
     * @return {unsigned short}
     */
    unsigned short get_port() const {
        return port_;
    }

    /**
     * 返回根据 URL 提取的相对路径部分（不含 ? 后面的参数）
     * @return {const char*}
     */
    const char* get_url_path() const {
        return url_path_.c_str();
    }

    /**
     * 返回从 URL 中提取的参数字段
     * @return {const char*}
     */
    const char* get_url_params() const {
        return url_params_.c_str();
    }

    /**
     * 清理解析过程中的中间状态，以便重复使用该类对象解析下一个 URL
     */
    void reset() {
        strncpy(proto_, "http", sizeof(proto_));
        port_ = 80;
        domain_.clear();
        url_path_.clear();
        url_params_.clear();
    }

private:
    bool parse_url_part(const char* url) {
        if (*url != '/') {
            return false;
        }

        const char* ptr = url;
        SKIP_WHILE(*ptr == '/', ptr);
        if (*ptr == 0) {
            url_path_ = "/";
            return true;
        }

        const char* qm = strchr(ptr, '?');
        if (qm == nullptr) {
            url_path_ = url;
            return true;
        }
        char buf[strlen(url)];
        strncpy(buf, url, qm - url);
        url_path_ = buf;
        ++qm;
        if (*qm != 0) {
            url_params_ = qm;
        }
        return true;
    }
    const char* parse_domain(const char* url) {
        if (*url == '/') {
            return nullptr;
        }
        const char* ptr = strchr(url, '/');
        if (ptr == nullptr) {
            domain_ = url;
            return nullptr;
        }

        char buf[256];
        size_t size = ptr - url + 1;
        if (size > sizeof(buf)) {
            size = sizeof(buf);
        }
        strncpy(buf, url, size);


        char* col = strchr(buf, ':');
        if (col != nullptr) {
            *col++ = 0;
            port_ = (unsigned short) atoi(col);
            if (port_ == 0 || port_ == 65535) {
                port_ = 80;
            }
        }
        domain_ = buf;
        return ptr;
    }

private:
    char proto_[16]{};
        string domain_;
        unsigned short port_;
        string url_path_;
        string url_params_;
};

#endif //ARM_FACE_IDENTIFY_HTTPURL_HPP
