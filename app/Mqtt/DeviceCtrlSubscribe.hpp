//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
#define ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP

#include "../../app_framework/Mqtt/SubscriberAbstract.h"
#include "../../app_framework/Mqtt/Helper.hpp"
#include "../../app_framework/Util/Base64.hpp"
#include "../Face/Identify.hpp"
#include "../Define.h"

class DeviceCtrlSubscribe : virtual public SubscriberAbstract{
public:
    explicit DeviceCtrlSubscribe(Config config, Identify *identify) : SubscriberAbstract(config), identify(identify) {
        this->identify->setPredictMatMapCallback(std::bind(&DeviceCtrlSubscribe::predictedMatCallback, this, std::placeholders::_1, std::placeholders::_2));
        this->matIdentifyTmpFileDir = Filesystem::getCurUserDocDir() + "/" + this->config.device.appName + "/runtimes/identify/";
        if (!Filesystem::dirExists(this->matIdentifyTmpFileDir )) {
            Filesystem::createDir(this->matIdentifyTmpFileDir);
        }
    }
    ~DeviceCtrlSubscribe() = default;

    string getTopic() override {
        return Helper::getDeviceCtrlTopic(this->config.device.appServerNamespace, this->config.device.appId);
    }

    void predictedMatCallback(ArmFaceIdentify::PredictMat predictMat, string flag) {
        string remoteUrl;
        string matFilePath = this->matIdentifyTmpFileDir + Util::randomStr(16) + JPG_EXT;
        try {
            imwrite(matFilePath, predictMat.sourceMat);

            nlohmann::json result = this->getHttpClient()
            ->uploadFile(this->config.server.httpServerAddress + "/api/util/attach/upload/image", matFilePath, {}, {}, true);

            remoteUrl = result.at("url").get<std::string>();
            Filesystem::unlink(matFilePath);
        } catch (std::exception &e) {
            if (Filesystem::fileExists(matIdentifyTmpFileDir)) Filesystem::unlink(matFilePath);

            this->exceptionHandler(e);
            return;
        }

        google_function::CloudEvent cloudEvent = CloudEvent::makeNewCloudEvent(this->config.device.appId, this->config.device.appName, APP_OPERATE_IDENTIFY);

        nlohmann::json payload;
        if (!flag.empty()) {
            payload["flag"] = flag;
        }
        payload["identify_label"] = predictMat.label;
        payload["identify_mat"] = remoteUrl;
        cloudEvent.set_data(to_string(payload));

        Helper::publishReportMsg(this->publishClient->getClient(), this->getDevice(), cloudEvent, this->exceptionHandler);
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
            if (Filesystem::dirExists(modelDir)) {
                Filesystem::removeDir(modelDir);
            }

            throw e;
        }

        Helper::publishReplySuccessMsg(this->publishClient->getClient(), this->getDevice(), cloudEvent, this->exceptionHandler);
    }

    void deleteFaceModel(google_function::CloudEvent cloudEvent) {
        nlohmann::json jsonObj = nlohmann::json::parse(cloudEvent.data());
        if (!jsonObj.contains("label")) {
            throw nlohmann::json::other_error::create(400,
                                                      "JSON message missing `label` fields", nullptr);
        }

        this->identify->deleteFaceModel(jsonObj.at("label").get<int64_t>());

        Helper::publishReplySuccessMsg(this->publishClient->getClient(), this->getDevice(), cloudEvent, this->exceptionHandler);
    }

    void onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) override {
        if (cloudEvent.type() == APP_OPERATE_ADD_FACE_MODEL) {
            this->addFaceModel(cloudEvent);
        }

        if (cloudEvent.type() == APP_OPERATE_DELETE_FACE_MODEL) {
            this->deleteFaceModel(cloudEvent);
        }
    }

protected:
    Identify *identify;
    string matIdentifyTmpFileDir;
};

#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
