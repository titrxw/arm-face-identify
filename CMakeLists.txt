cmake_minimum_required(VERSION 3.17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(projectName arm_face_identify)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# Define project name
project(${projectName})

add_executable(${projectName}
        main.cpp core/identify/Source/FaceIdentify.cpp core/identify/Include/FaceIdentify.h core/identify/Include/Identify.h core/identify/Source/Identify.cpp core/train/Source/FaceTrain.cpp core/train/Include/FaceTrain.h core/train/Include/Train.h core/train/Source/Train.cpp core/helper/Include/Str.h core/helper/Source/Str.cpp core/train/Source/DialogVideoFaceTrain.cpp core/train/Include/DialogVideoFaceTrain.h core/base/Source/Opencv.cpp core/base/Include/Opencv.h core/helper/Source/File.cpp core/helper/Include/File.h core/base/Enum/Event.h  core/base/Include/Event/BaseEvent.h core/base/Include/Event/DetectedFeatureMatEvent.h core/train/Source/DialogVideoFaceTrain.cpp core/train/Include/DialogVideoFaceTrain.h core/entity/Include/PredictMat.h core/identify/Source/DialogVideoFaceIdentify.cpp core/identify/Include/DialogVideoFaceIdentify.h core/base/Include/Event/PredictFeatureMatEvent.h core/entity/Include/DetectedMat.h
        app/Face/Train.cpp app/Face/Train.h app/Face/Identify.cpp app/Face/Identify.h)

MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "current system: ${CMAKE_SYSTEM_NAME}")
message(STATUS "current processor: ${CMAKE_SYSTEM_PROCESSOR}")

## library
find_package(PkgConfig REQUIRED)

### mqtt
link_directories(vendor/mqtt/lib)
include_directories(vendor/mqtt/include)

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
        target_link_libraries(${projectName} vendor/mqtt/lib/libpaho-mqttpp3.1.dylib)
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

    target_link_libraries(${projectName} vendor/mqtt/lib/libpaho-mqttpp3.so)
endif()

## vendor
include_directories("./vendor")

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