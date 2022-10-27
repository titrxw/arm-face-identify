//
// Created by rxwyun on 2022/8/1.
//

#ifndef ARM_IOT_EXCEPTIONHANDLER_HPP
#define ARM_IOT_EXCEPTIONHANDLER_HPP

#include "exception"
#include "spdlog/spdlog.h"

namespace IOT {
    class ExceptionHandler {
    public:
        ExceptionHandler(std::shared_ptr<spdlog::logger> logger) : logger(logger) {

        }

        void report(std::exception &e) {
            this->logger->debug(e.what());
            this->logger->flush();
        }

        void handle(std::exception &e) {
            this->report(e);
        }

    protected:
        std::shared_ptr<spdlog::logger> logger;
    };
}

#endif //ARM_IOT_EXCEPTIONHANDLER_HPP
