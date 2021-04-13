cmake_minimum_required(VERSION 3.17)
project(ArmFaceIdentify)

set(CMAKE_CXX_STANDARD 14)

add_executable(ArmFaceIdentify
        identify/main.cpp
        train/main.cpp)
