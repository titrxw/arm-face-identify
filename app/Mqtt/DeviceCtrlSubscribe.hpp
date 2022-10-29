//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
#define ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP

#include "../../app_framework/Client/SubscriberAbstract.hpp"
#include "../../app_framework/Util/Base64.hpp"
#include "../Face/Identify.hpp"
#include "../Define.h"
#include "../Helper.hpp"
#include "../../app_framework/Http/HttpClient.hpp"

class DeviceCtrlSubscribe : virtual public IOT::CLIENT::SubscriberAbstract{
public:
    explicit DeviceCtrlSubscribe(IOT::CONFIG::Device device, Identify *identify, IOT::CLIENT::ClientAbstract *publishClient, string httpServerAddress) : SubscriberAbstract(device), identify(identify), publishClient(publishClient), httpServerAddress(httpServerAddress) {
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
            ->uploadFile(this->httpServerAddress + "/api/util/attach/upload/image", matFilePath, {}, {}, true);

            remoteUrl = result.at("url").get<std::string>();
            IOT::UTIL::Filesystem::unlink(matFilePath);
        } catch (std::exception &e) {
            if (IOT::UTIL::Filesystem::fileExists(matIdentifyTmpFileDir)) IOT::UTIL::Filesystem::unlink(matFilePath);

            this->exceptionHandler(e);
            return;
        }

        google_function::CloudEvent cloudEvent = IOT::UTIL::CloudEvent::makeNewCloudEvent(this->device.appId, this->device.appName, APP_OPERATE_IDENTIFY);

        nlohmann::json payload;
        if (!flag.empty()) {
            payload["flag"] = flag;
        }
        payload["label"] = predictMat.label;
        payload["mat"] = remoteUrl;
        cloudEvent.set_data(to_string(payload));

        Helper::publishReportMsg(this->publishClient, this->device, cloudEvent, this->exceptionHandler);
    }

    void addFaceModel(google_function::CloudEvent cloudEvent) {
        nlohmann::json jsonObj = nlohmann::json::parse(cloudEvent.data());
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

        Helper::publishReplySuccessMsg(this->publishClient, this->device, cloudEvent, this->exceptionHandler);
    }

    void deleteFaceModel(google_function::CloudEvent cloudEvent) {
        nlohmann::json jsonObj = nlohmann::json::parse(cloudEvent.data());
        if (!jsonObj.contains("label")) {
            throw nlohmann::json::other_error::create(400,
                                                      "JSON message missing `label` fields", nullptr);
        }

        this->identify->deleteFaceModel(jsonObj.at("label").get<int64_t>());

        Helper::publishReplySuccessMsg(this->publishClient, this->device, cloudEvent, this->exceptionHandler);
    }

    void onSubscribe(IOT::CLIENT::ClientAbstract *client, google_function::CloudEvent cloudEvent) override {
        if (cloudEvent.type() == APP_OPERATE_ADD_FACE_MODEL) {
            this->addFaceModel(cloudEvent);
        }

        if (cloudEvent.type() == APP_OPERATE_DELETE_FACE_MODEL) {
            this->deleteFaceModel(cloudEvent);
        }
    }

protected:
    IOT::CLIENT::ClientAbstract *publishClient = nullptr;
    Identify *identify = nullptr;
    IOT::CONFIG::Device device;
    string matIdentifyTmpFileDir;
    IOT::HTTP::HttpClient *httpClient = nullptr;
    string httpServerAddress;
};

#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
