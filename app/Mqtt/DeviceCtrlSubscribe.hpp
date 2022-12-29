//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
#define ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP

#include <utility>

#include "../../app_framework/Client/SubscriberAbstract.hpp"
#include "../../app_framework/Util/Base64.hpp"
#include "../Face/Identify.hpp"
#include "../Define.h"
#include "../Helper.hpp"
#include "../../app_framework/Http/HttpClient.hpp"

class DeviceCtrlSubscribe : virtual public IOT::CLIENT::SubscriberAbstract{
public:
    explicit DeviceCtrlSubscribe(IOT::CONFIG::Device device, Identify *identify, IOT::CLIENT::ClientAbstract *publishClient, string httpServerAddress) : SubscriberAbstract(std::move(device)), identify(identify), publishClient(publishClient), httpServerAddress(httpServerAddress) {
        this->identify->setPredictMatMapCallback(std::bind(&DeviceCtrlSubscribe::predictedMatCallback, this, std::placeholders::_1, std::placeholders::_2));
        this->matIdentifyTmpFileDir = IOT::UTIL::Filesystem::getCurUserDocDir() + "/" + this->device.appName + "/runtimes/identify/";
        if (!IOT::UTIL::Filesystem::dirExists(this->matIdentifyTmpFileDir )) {
            IOT::UTIL::Filesystem::createDir(this->matIdentifyTmpFileDir);
        }
    }
    ~DeviceCtrlSubscribe() = default;

    string getTopic() override {
        return Helper::getDeviceCtrlTopic(this->device.appServerNamespace, this->device.appId);
    }

    IOT::HTTP::HttpClient *getHttpClient() {
        if (this->httpClient == nullptr) {
            this->httpClient = (new IOT::HTTP::HttpClient())->withAppId(this->device.appId)->withAppSecret(this->device.appSecret);
        }

        return this->httpClient;
    }

    void predictedMatCallback(ArmFaceIdentify::PredictMat predictMat, string flag) {
        string remoteUrl;
        string matFilePath = this->matIdentifyTmpFileDir + IOT::UTIL::Util::randomStr(16) + JPG_EXT;
        try {
            imwrite(matFilePath, predictMat.sourceMat);

            nlohmann::json result = this->getHttpClient()
            ->uploadFile(this->httpServerAddress + "/api/v1/util/attach/upload/image", matFilePath, {}, {}, true);

            remoteUrl = result.at("url").get<std::string>();
            IOT::UTIL::Filesystem::unlink(matFilePath);
        } catch (std::exception &e) {
            if (IOT::UTIL::Filesystem::fileExists(matIdentifyTmpFileDir)) IOT::UTIL::Filesystem::unlink(matFilePath);

            this->exceptionHandler(e);
            return;
        }

        IOT::MESSAGE::IotMessage message;
        message.eventType = APP_OPERATE_IDENTIFY;
        nlohmann::json payload;
        if (!flag.empty()) {
            payload["flag"] = flag;
        }
        payload["label"] = predictMat.label;
        payload["mat"] = remoteUrl;
        message.payload = to_string(payload);

        Helper::publishReportMsg(this->publishClient, this->device, message, this->exceptionHandler);
    }

    void addFaceModel(const IOT::MESSAGE::IotMessage& message) {
        nlohmann::json jsonObj = nlohmann::json::parse(message.payload);
        if (!jsonObj.contains("label") || !jsonObj.contains("urls")) {
            throw nlohmann::json::other_error::create(400,
                                                      "JSON message missing `label`, `urls` fields", nullptr);
        }

        int label = jsonObj.at("label").get<int64_t>();
        try {
            this->identify->addFaceModelFromRemoteImgUrls(label, jsonObj.at("urls").get<vector<std::string>>());
        } catch (std::exception &e) {
            string modelDir = this->identify->getFaceModelImgSavePathWithLabel(label);
            if (IOT::UTIL::Filesystem::dirExists(modelDir)) {
                IOT::UTIL::Filesystem::removeDir(modelDir);
            }

            throw e;
        }

        Helper::publishReplySuccessMsg(this->publishClient, this->device, message, this->exceptionHandler);
    }

    void deleteFaceModel(const IOT::MESSAGE::IotMessage& message) {
        nlohmann::json jsonObj = nlohmann::json::parse(message.payload);
        if (!jsonObj.contains("label")) {
            throw nlohmann::json::other_error::create(400,
                                                      "JSON message missing `label` fields", nullptr);
        }

        this->identify->deleteFaceModel(jsonObj.at("label").get<int64_t>());

        Helper::publishReplySuccessMsg(this->publishClient, this->device, message, this->exceptionHandler);
    }

    void onSubscribe(IOT::CLIENT::ClientAbstract *client, IOT::MESSAGE::IotMessage message) override {
        if (message.eventType == APP_OPERATE_ADD_FACE_MODEL) {
            this->addFaceModel(message);
        }

        if (message.eventType == APP_OPERATE_DELETE_FACE_MODEL) {
            this->deleteFaceModel(message);
        }
    }

protected:
    IOT::CLIENT::ClientAbstract *publishClient = nullptr;
    Identify *identify = nullptr;
    string matIdentifyTmpFileDir;
    IOT::HTTP::HttpClient *httpClient = nullptr;
    string httpServerAddress;
};

#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
