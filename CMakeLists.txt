cmake_minimum_required(VERSION 3.17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(projectName arm_face_identify)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Define project name
project(${projectName})

add_executable(${projectName}
        main.cpp
        identify_core/identify/Source/FaceIdentify.cpp
        identify_core/identify/Include/FaceIdentify.h
        identify_core/identify/Include/Identify.h
        identify_core/identify/Source/Identify.cpp
        identify_core/train/Source/FaceTrain.cpp
        identify_core/train/Include/FaceTrain.h
        identify_core/train/Include/Train.h
        identify_core/train/Source/Train.cpp
        identify_core/helper/Include/Str.h
        identify_core/helper/Source/Str.cpp
        identify_core/train/Source/DialogVideoFaceTrain.cpp
        identify_core/train/Include/DialogVideoFaceTrain.h
        identify_core/base/Source/Opencv.cpp
        identify_core/base/Include/Opencv.h
        identify_core/helper/Source/File.cpp
        identify_core/helper/Include/File.h
        identify_core/base/Enum/Event.h
        identify_core/base/Include/Event/BaseEvent.h
        identify_core/base/Include/Event/DetectedFeatureMatEvent.h
        identify_core/train/Source/DialogVideoFaceTrain.cpp
        identify_core/train/Include/DialogVideoFaceTrain.h
        identify_core/entity/Include/PredictMat.h
        identify_core/identify/Source/DialogVideoFaceIdentify.cpp
        identify_core/identify/Include/DialogVideoFaceIdentify.h
        identify_core/base/Include/Event/PredictFeatureMatEvent.h
        identify_core/entity/Include/DetectedMat.h
#

        app_framework/Mqtt/Client.cpp
        app_framework/Mqtt/Client.h
        app_framework/Mqtt/SubscriberAbstract.cpp
        app_framework/Mqtt/SubscriberAbstract.h
        app_framework/Mqtt/SubscribeManager.cpp
        app_framework/Mqtt/SubscribeManager.h
        app_framework/Mqtt/ClientCallback.hpp
        app_framework/Mqtt/Helper.hpp
        app_framework/Util/Encrypt.hpp
        app_framework/Util/CloudEvent.hpp
        app_framework/Util/Filesystem.hpp
        app_framework/Exception/ExceptionHandler.hpp
        app_framework/Application.cpp
        app_framework/Application.h

        app/Face/Train.cpp
        app/Face/Train.h
        app/Face/Identify.cpp
        app/Face/Identify.h
        app/IdentifyApplication.cpp
        app/IdentifyApplication.h
        app/Mqtt/DeviceCtrlSubscribe.hpp
        config/Config.h
        config/Mqtt.h
        config/Device.h
        config/App.h
        config/FaceIdentify.h
        app/Define.h app_framework/Util/Base64.hpp)

message(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "current system: ${CMAKE_SYSTEM_NAME}")
message(STATUS "current processor: ${CMAKE_SYSTEM_PROCESSOR}")

## library
find_package(PkgConfig REQUIRED)

## vendor
include_directories("${PROJECT_SOURCE_DIR}/vendor")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux|Darwin" OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES "^(arm.*|ARM.*)")
    ## opencv
    find_package(OpenCV 3.4.14 REQUIRED)
    message(STATUS "OpenCV library status:")
    message(STATUS "    config: ${OpenCV_DIR}")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_directories(${OpenCV_LIB_DIR})
    target_link_libraries(${projectName} ${OpenCV_LIBS})

    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        link_directories("${PROJECT_SOURCE_DIR}/vendor/mqtt/lib")
        target_link_libraries(${projectName} "${PROJECT_SOURCE_DIR}/vendor/mqtt/lib/libpaho-mqtt3a.1.3.8.dylib")
        target_link_libraries(${projectName} "${PROJECT_SOURCE_DIR}/vendor/mqtt/lib/libpaho-mqttpp3.dylib")
        target_link_libraries(${projectName} "${PROJECT_SOURCE_DIR}/vendor/cryptopp/lib/libcryptopp_m.so")
        target_link_libraries(${projectName} "${PROJECT_SOURCE_DIR}/vendor/opencv/lib/mac/libopencv_face.3.4.14.dylib")
    endif()

endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES "^(arm.*|ARM.*)")
    ### gtk
    pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
    message(STATUS "GTK2 library status:")
    message(STATUS "    GTK2_INCLUDE_DIRS: ${GTK2_INCLUDE_DIRS}")
    message(STATUS "    GTK2_LIBRARIES: ${GTK2_LIBRARIES}")
    message(STATUS "    GTK2_LINK_LIBRARIES: ${GTK2_LINK_LIBRARIES}")
    include_directories(${GTK2_INCLUDE_DIRS})
    link_directories(${GTK2_LIBRARY_DIRS})
    list(APPEND FC_DEP_LIBS ${GTK2_LIBRARIES})

    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^(arm.*|ARM.*)")
        ### atomic
        target_link_libraries(${projectName} atomic)
    endif()

    target_link_libraries(${projectName} "${PROJECT_SOURCE_DIR}/vendor/mqtt/lib/libpaho-mqtt3as.so")
    target_link_libraries(${projectName} "${PROJECT_SOURCE_DIR}/vendor/mqtt/lib/libpaho-mqttpp3.so.1")
    target_link_libraries(${projectName} "${PROJECT_SOURCE_DIR}/vendor/cryptopp/lib/libcryptopp_l.so")
    target_link_libraries(${projectName} "${PROJECT_SOURCE_DIR}/vendor/cryptopp/lib/libcryptopp.so.8")
    target_link_libraries(${projectName} "${PROJECT_SOURCE_DIR}/vendor/opencv/lib/linux/libopencv_face.so")
endif()

#### boost/filesystem.so  boost v1.77.0 boost build完成后把filesystem.so复制到vendor目录
#add_library(boost_filesystem
#        SHARED
#        IMPORTED)
#set_target_properties(boost_filesystem
#        PROPERTIES IMPORTED_LOCATION
#        vendor/boost/libboost_filesystem.so)

#add_subdirectory("data")

# Declare the executable target built from your sources

# target_link_libraries(${projectName} PRIVATE boost_filesystem)

# cmake -DCMAKE_INSTALL_PREFIX=/usr/local/arm-face-identify  ..
# 在执行make install 的时候会把data目录下的文件复制到 CMAKE_INSTALL_PREFIX 目录下的data目录

#树莓派
# cmake -DCMAKE_INSTALL_PREFIX=/usr/local/arm-face-identify -DCMAKE_CXX_STANDARD=11 -DCMAKE_SHARED_LINKER_FLAGS='-latomic'  ..

install(DIRECTORY data/ DESTINATION data)