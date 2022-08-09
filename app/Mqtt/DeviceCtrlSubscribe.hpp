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

class DeviceCtrlSubscribe : virtual public SubscriberAbstract{
public:
    explicit DeviceCtrlSubscribe(Config config, Identify *identify, Train *train) : SubscriberAbstract(config), identify(identify), train(train) {
        this->identify->setPredictMatMapCallback(std::bind(&DeviceCtrlSubscribe::predictedMatCallback, this, std::placeholders::_1, std::placeholders::_2));
        this->matTmpFileDir = Filesystem::getCurUserDocDir() + "/" + this->config.device.appName + "/runtimes/identify/";
        if (!Filesystem::dirExists(this->matTmpFileDir )) {
            Filesystem::createDir(this->matTmpFileDir);
        }
    }
    ~DeviceCtrlSubscribe() = default;

    void predictedMatCallback(ArmFaceIdentify::PredictMat predictMat, string flag) {
        string remoteUrl;
        string matFilePath = this->matTmpFileDir + Util::randomStr(16) + ".jpeg";
        try {
            imwrite(matFilePath, predictMat.sourceMat);

            nlohmann::json result = this->getHttpClient()
            ->uploadFile("/api/util/attach/upload/image", matFilePath, {}, {}, true);

            remoteUrl = result.at("url").get<std::string>();
            Filesystem::unlink(matFilePath);
        } catch (std::exception &e) {
            if (Filesystem::fileExists(matTmpFileDir)) Filesystem::unlink(matFilePath);

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

    string getTopic() override {
        return Helper::getDeviceCtrlTopic(this->config.device.appServerNamespace, this->config.device.appId);
    }

    void onSubscribe(async_client *client, const_message_ptr msg, google_function::CloudEvent cloudEvent) override {
        if (cloudEvent.type() == APP_OPERATE_TRAIN) {

        }
    }

protected:
    Identify *identify;
    Train *train;
    string matTmpFileDir;
};

#endif //ARM_FACE_IDENTIFY_DEVICECTRLSUBSCRIBE_HPP
