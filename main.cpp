#include "train/Include/FaceTrain.h"
#include <unistd.h>
#include "helper/Include/Str.h"
#include <ctime>

using namespace ArmFaceIdentify;

int main()
{
    char* tmpCurPwd = nullptr;
    tmpCurPwd = getcwd(nullptr, 0);
    string curPwd(tmpCurPwd);
    curPwd = curPwd.append("/../test/");

    string sourceFile(curPwd);
    string targetFile(curPwd);
    sourceFile = sourceFile.append("at.txt");
    targetFile = targetFile.append(Str::lToString(time(nullptr))).append("_face_model.xml");

    FaceTrain* faceTrain = new FaceTrain();
    faceTrain->trainAndSave(sourceFile, targetFile);

    delete [] tmpCurPwd;
    tmpCurPwd = nullptr;
    delete faceTrain;
    faceTrain = nullptr;

    return 0;
}