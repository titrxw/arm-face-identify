#include <fstream>
#include "app/IdentifyApplication.h"
#include "config/Config.h"

int main()
{
    Config *config = new Config();
    std::ifstream ifs(IOT::UTIL::Filesystem::getCurUserDocDir() + "/" + APP_NAME + "/config.json");

    json j;
    ifs >> j;
    *config = (Config)j;
    config->device.appName = APP_NAME;

    (new IdentifyApplication(config))->start();

    return 0;
}