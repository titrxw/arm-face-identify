#include <fstream>
#include "app/IdentifyApplication.h"
#include "config/Config.h"
#include "config/Device.h"

int main()
{
    Config config;
    std::ifstream ifs("/Users/rxwyun/Develop/c/arm-face-identify/config.json");

    json j;
    ifs >> j;
    config = (Config)j;

    (new IdentifyApplication(config))->start();

    return 0;
}