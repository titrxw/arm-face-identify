#include <fstream>
#include "app/IdentifyApplication.h"
#include "config/Config.h"
#include "config/Device.h"

int main()
{
    Config config;
    std::ifstream ifs(Filesystem::getCurUserDocDir() + "/" + APP_NAME + "/config.json");

    json j;
    ifs >> j;
    config = (Config)j;
    config.device.appName = APP_NAME;

    (new IdentifyApplication(config))->start();

    return 0;
}