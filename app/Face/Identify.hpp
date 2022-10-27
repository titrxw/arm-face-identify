//
// Created by rxwyun on 2022/7/13.
//

#ifndef ARM_FACE_IDENTIFY_IDENTIFY_APP_H
#define ARM_FACE_IDENTIFY_IDENTIFY_APP_H

#include <sstream>
#include "vector"
#include <time.h>
#include "opencv2/face.hpp"
#include "opencv2/opencv.hpp"
#include "../../identify_core/identify/Include/DialogVideoFaceIdentify.h"
#include "../../identify_core/train/Include/FaceTrain.h"
#include "../../app_framework/Util/Filesystem.hpp"
#include "../../app_framework/Http/HttpClient.hpp"

using namespace cv;
using namespace cv::face;
using namespace std;

const string JPG_EXT = ".jpeg";

class Identify
{
public:
    Identify(const string& cascadeFilePath, string modelSaveDir, int videoCaptureIndex = 0) : cascadeFilePath(cascadeFilePath), modelSaveDir(modelSaveDir), videoCaptureIndex(videoCaptureIndex) {
        if (!IOT::UTIL::Filesystem::fileExists(this->cascadeFilePath)) {
            throw std::logic_error("cascade file " + this->cascadeFilePath + " not exists");
        }

        if (!IOT::UTIL::Filesystem::dirExists(this->modelSaveDir)) {
            IOT::UTIL::Filesystem::createDir(this->modelSaveDir);
        }

        string modelImgSavePath = this->getFaceModelImgSavePath();
        if (!IOT::UTIL::Filesystem::dirExists(modelImgSavePath)) {
            IOT::UTIL::Filesystem::createDir(modelImgSavePath);
        }

        this->eventDispatcher = new EventDispatcher<int, void (ArmFaceIdentify::BaseEvent *event)>();
    }
    ~Identify() {
        if (this->cascade){
            this->cascade.release();
        }
        if (this->modelRecognizer) {
            this->modelRecognizer.release();
        }

        delete this->eventDispatcher;
        this->eventDispatcher = nullptr;

        if (this->faceIdentifyHandler != nullptr) {
            delete this->faceIdentifyHandler;
            this->faceIdentifyHandler = nullptr;
        }
        if (this->faceTrainHandler != nullptr) {
            delete this->faceTrainHandler;
            this->faceTrainHandler = nullptr;
        }
    }

    string getFaceDefaultModelFilePath() {
        return this->modelSaveDir  + "default_model.xml";
    }

    string getAppFaceModelFilePath() {
        return this->modelSaveDir  + "identify_model.xml";
    }

    void setPredictMatConfidence(double predictMatConfidence) {
        this->predictMatConfidence = predictMatConfidence;
    }

    void setPredictMatMapCallback(std::function<void (ArmFaceIdentify::PredictMat, string)> callback) {
        this->predictMatCallback = std::move(callback);
    }

    Ptr<CascadeClassifier> getCascadeClassifier() {
        if (this->cascade == nullptr) {
            this->cascade = new CascadeClassifier(this->cascadeFilePath);
        }

        return this->cascade;
    }

    Ptr<FaceRecognizer> getModelRecognizer() {
        if (this->modelRecognizer == nullptr) {
            this->modelRecognizer = LBPHFaceRecognizer::create();
            this->reloadModelRecognizer();
        }

        return this->modelRecognizer;
    }

    void reloadModelRecognizer() {
        string appModelPath = this->getAppFaceModelFilePath();

        this->modelRecognizer->clear();
        if (IOT::UTIL::Filesystem::fileExists(appModelPath)) {
            this->modelRecognizer->read(appModelPath);
        } else {
            string defaultModelPath = this->getFaceDefaultModelFilePath();
            this->modelRecognizer->read(defaultModelPath);
        }
    }

    ArmFaceIdentify::DialogVideoFaceIdentify *getFaceIdentifyHandler() {
        if (this->faceIdentifyHandler == nullptr) {
            this->faceIdentifyHandler = new ArmFaceIdentify::DialogVideoFaceIdentify(this->getCascadeClassifier(), this->getModelRecognizer(), this->eventDispatcher);
            this->faceIdentifyHandler->setPredictMatMapCallback([this](vector<ArmFaceIdentify::PredictMat> predictMatMap, string flag) {
                if (predictMatMap.size() > 1) {
                    return;
                }

                if (predictMatMap[0].confidence > this->predictMatConfidence){
                    return;
                }

                time_t curTime = time(NULL);
                int timeStep = abs(curTime - this->curIdentifyLatestTime);
                if (this->curIdentifyLabel == predictMatMap[0].label && timeStep < 60) {
                    return;
                }

                this->curIdentifyLabel = predictMatMap[0].label;
                this->curIdentifyLatestTime = curTime;
                if (this->predictMatCallback != nullptr) {
                    this->predictMatCallback(predictMatMap[0], flag);
                }
            });
        }

        return this->faceIdentifyHandler;
    }

    ArmFaceIdentify::FaceTrain *getFaceTrainHandler() {
        if (this->faceTrainHandler == nullptr) {
            this->faceTrainHandler = new ArmFaceIdentify::FaceTrain(this->getCascadeClassifier(), this->getModelRecognizer(), this->eventDispatcher);
        }

        return this->faceTrainHandler;
    }

    string getFaceModelImgSavePath() {
        return this->modelSaveDir + "img/";
    }

    string getFaceModelImgSavePathWithLabel(int label) {
        return this->modelSaveDir + "img/" + to_string(label) + "/";
    }

    void addFaceModelFromRemoteImgUrls(int label, vector<std::string> remoteUrls) {
        int remoteUrlSize = remoteUrls.size();
        if (remoteUrlSize < 8) {
            throw std::logic_error("训练图片数量不能小于8张");
        }

        string trainDir = this->getFaceModelImgSavePathWithLabel(label);
        if (!IOT::UTIL::Filesystem::dirExists(trainDir)) {
            IOT::UTIL::Filesystem::createDir(trainDir);
        }

        vector<std::string> localPaths;
        for (int i = 0; i < remoteUrlSize; ++i) {
            string filePath = trainDir + IOT::UTIL::Encrypt::md5(remoteUrls[i]) + JPG_EXT;

            if (IOT::HTTP::HttpClient::downloadFile(remoteUrls[i], filePath)) {
                localPaths.push_back(filePath);
            }
        }
        remoteUrls.clear();
        int localPathSize = localPaths.size();
        localPaths.clear();

        if (remoteUrlSize == localPathSize) {
            this->reTrainFaceModel();
            return;
        }

        throw std::logic_error("图片下载失败");
    }

    void deleteFaceModel(int label) {
        string dirPath = this->getFaceModelImgSavePathWithLabel(label);
        if (IOT::UTIL::Filesystem::dirExists(dirPath)) {
            IOT::UTIL::Filesystem::removeDir(dirPath);

            this->reTrainFaceModel();
        }
    }

    void reTrainFaceModel() {
        string modelImgSavePath = this->getFaceModelImgSavePath();
        vector<string> dirs = IOT::UTIL::Filesystem::getChildDirs(modelImgSavePath);
        int dirSize = dirs.size();
        string modelFileContent;
        for (int i = 0; i < dirSize; ++i) {
            int label = atoi(dirs[i].c_str());
            if (label > 0) {
                vector<string> files = IOT::UTIL::Filesystem::getDirFiles(modelImgSavePath + dirs[i]);
                int fileSize = files.size();
                if (fileSize <= 0) {
                    continue;
                }
                for (int j = 0; j < fileSize; ++j) {
                    modelFileContent = modelFileContent.append(modelImgSavePath + dirs[i] + "/" + files[j]).append(";").append(to_string(label)).append("\n");
                }
                files.clear();
            }
        }
        dirs.clear();
        if (modelFileContent.empty()) {
            string appModelPath = this->getAppFaceModelFilePath();
            if (IOT::UTIL::Filesystem::fileExists(appModelPath)) {
                IOT::UTIL::Filesystem::unlink(appModelPath);
            }

            this->reloadModelRecognizer();
            return;
        }

        string sampleFilePath = this->modelSaveDir +  "model.txt";
        IOT::UTIL::Filesystem::write(sampleFilePath, modelFileContent);

        try {
            this->getFaceTrainHandler()->trainAndSave(sampleFilePath, this->getAppFaceModelFilePath());
            IOT::UTIL::Filesystem::unlink(sampleFilePath);
        } catch (std::exception &e) {
            IOT::UTIL::Filesystem::unlink(sampleFilePath);
            throw e;
        }
    }

    void startIdentifyFromVideoCapture() {
        VideoCapture capture(this->videoCaptureIndex);
        this->getFaceIdentifyHandler()->identifyFromVideoCapture(&capture);
        capture.release();
    }

protected:
    string cascadeFilePath;
    string modelSaveDir;
    int videoCaptureIndex;
    double predictMatConfidence;
    Ptr<CascadeClassifier> cascade;
    Ptr<FaceRecognizer> modelRecognizer;
    EventDispatcher<int, void(ArmFaceIdentify::BaseEvent *event)> *eventDispatcher = nullptr;
    ArmFaceIdentify::DialogVideoFaceIdentify *faceIdentifyHandler = nullptr;
    ArmFaceIdentify::FaceTrain *faceTrainHandler = nullptr;
    std::function<void (ArmFaceIdentify::PredictMat, string)> predictMatCallback;

    int curIdentifyLabel = 0;
    time_t curIdentifyLatestTime;
};

#endif // ARM_FACE_IDENTIFY_IDENTIFY_H
