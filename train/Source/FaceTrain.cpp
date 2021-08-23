//
// Created by rxwyun on 2021/8/22.
//

#include "../Include/FaceTrain.h"
#include <fstream>

void ArmFaceIdentify::FaceTrain::loadSourceFile(const string &filename, vector<Mat> &mats, vector<int> &matLabels, char separator)
{
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file)
    {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, label;
    while (getline(file, line))
    {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, label);
        if (!path.empty() && !label.empty())
        {
            Mat img = imread(path, 0);
            resize(img, img, Size(92, 112));
            mats.push_back(img);
            matLabels.push_back(atoi(label.c_str()));
        }
    }
}

void ArmFaceIdentify::FaceTrain::trainMats(vector<Mat> &mats, vector<int> &matLabels, const string &targetFile)
{
    Ptr<BasicFaceRecognizer> model = EigenFaceRecognizer::create();
    model->train(mats, matLabels);
    model->save(targetFile);
}