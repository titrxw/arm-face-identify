//
// Created by rxwyun on 2022/7/24.
//

#ifndef ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
#define ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP

#include "../../app_framework/Mqtt/SubscriberAbstract.h"
#include "../../app_framework/Mqtt/Helper.hpp"
#include "../../app_framework/Util/Base64.hpp"
#include "../Face/Identify.h"
#include "../Face/Train.h"
#include "../Define.h"

const string JPG_EXT = ".jpeg";

class DeviceCtrlSubscribe : virtual public SubscriberAbstract{
public:
    explicit DeviceCtrlSubscribe(Config config, Identify *identify, Train *train) : SubscriberAbstract(config), identify(identify), train(train) {
        this->identify->setPredictMatMapCallback(std::bind(&DeviceCtrlSubscribe::predictedMatCallback, this, std::placeholders::_1, std::placeholders::_2));
        this->matIdentifyTmpFileDir = Filesystem::getCurUserDocDir() + "/" + this->config.device.appName + "/runtimes/identify/";
        if (!Filesystem::dirExists(this->matIdentifyTmpFileDir )) {
            Filesystem::createDir(this->matIdentifyTmpFileDir);
        }
        this->matTrainTmpFileDir = Filesystem::getCurUserDocDir() + "/" + this->config.device.appName + "/runtimes/train/";
        if (!Filesystem::dirExists(this->matTrainTmpFileDir )) {
            Filesystem::createDir(this->matTrainTmpFileDir);
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
            ->uploadFile("/api/util/attach/upload/image", matFilePath, {}, {}, true);

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

    void trainRemoteUrls(google_function::CloudEvent cloudEvent, int label, vector<std::string> remoteUrls) {
        string trainDir = this->matTrainTmpFileDir + to_string(label) + "/";
        if (!Filesystem::dirExists(trainDir)) {
            Filesystem::createDir(trainDir);
        }

        int remoteUrlSize = remoteUrls.size();
        if (remoteUrlSize < 8) {
            throw std::logic_error("训练图片数量不能小于8张");
        }

        vector<std::string> localPaths;
        localPaths.resize(remoteUrlSize);
        for (int i = 0; i < remoteUrlSize; ++i) {
            string filePath = trainDir + Encrypt::md5(remoteUrls[i]) + JPG_EXT;

            try {
                if (this->getHttpClient()->downloadFile(remoteUrls[i], filePath)) {
                    localPaths.push_back(filePath);
                }
            } catch (std::exception &e) {
                this->exceptionHandler(e);
                break;
            }
        }
        int localPathSize = localPaths.size();

        string modelPath;
        if (remoteUrlSize == localPathSize) {
            modelPath = this->train->trainFromRemoteImgUrls(label, localPaths);
        }

        for (int i = 0; i < localPathSize; ++i) {
            Filesystem::unlink(localPaths[i]);
        }

        if (!modelPath.empty()) {
            this->identify->getModelRecognizer()->read(modelPath);

            //success
            nlohmann::json payload;
            payload["result"] = "success";
            cloudEvent.set_data(to_string(payload));
            Helper::publishReplyMsg(this->publishClient->getClient(), this->getDevice(), cloudEvent, this->exceptionHandler);
        }
    }

    void onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) override {
        if (cloudEvent.type() == APP_OPERATE_ADD_FACE_MODEL) {
            nlohmann::json jsonObj = nlohmann::json::parse(cloudEvent.data());
            if (!jsonObj.contains("label") || !jsonObj.contains("urls")) {
                throw nlohmann::json::other_error::create(400,
                                                          "JSON message missing `label`, `urls` fields", nullptr);
            }

            try {
                this->trainRemoteUrls(cloudEvent, jsonObj.at("label").get<int64_t>(), jsonObj.at("urls").get<vector<std::string>>());
            } catch (std::exception &e) {
                this->exceptionHandler(e);
            }
        }
    }

protected:
    Identify *identify;
    Train *train;
    string matIdentifyTmpFileDir;
    string matTrainTmpFileDir;
};

#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
