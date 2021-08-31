cmake_minimum_required(VERSION 3.17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)


# Define project name
project(arm_face_identify)

### opencv
# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV 3.4.14 REQUIRED)
# If the package has been found,several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
LINK_DIRECTORIES(${OpenCV_LIB_DIR})

### vendor
INCLUDE_DIRECTORIES("./vendor")

#### boost/filesystem.so  boost v1.77.0 boost build完成后把filesystem.so复制到vendor目录
#add_library(boost_filesystem
#        SHARED
#        IMPORTED)
#set_target_properties(boost_filesystem
#        PROPERTIES IMPORTED_LOCATION
#        vendor/boost/libboost_filesystem.so)
#add_subdirectory("data")
# Declare the executable target built from your sources
add_executable(arm_face_identify
        main.cpp core/identify/Source/FaceIdentify.cpp core/identify/Include/FaceIdentify.h core/identify/Include/Identify.h core/identify/Source/Identify.cpp core/train/Source/FaceTrain.cpp core/train/Include/FaceTrain.h core/train/Include/Train.h core/train/Source/Train.cpp core/helper/Include/Str.h core/helper/Source/Str.cpp business/train/Source/DialogVideoFaceTrain.cpp business/train/Include/DialogVideoFaceTrain.h core/base/Source/Opencv.cpp core/base/Include/Opencv.h core/helper/Source/File.cpp core/helper/Include/File.h core/base/Enum/Event.h  core/base/Include/Event/BaseEvent.h core/base/Include/Event/DetectedFeatureMatEvent.h business/train/Source/DialogVideoFaceTrain.cpp business/train/Include/DialogVideoFaceTrain.h core/entity/Include/PredictFace.h business/identify/Source/DialogVideoFaceIdentify.cpp business/identify/Include/DialogVideoFaceIdentify.h core/base/Include/Event/PredictFeatureMatEvent.h core/entity/Include/DetectedFace.h
        )

# Link your application with OpenCV libraries
target_link_libraries(arm_face_identify PRIVATE ${OpenCV_LIBS})
# 树莓派
#target_link_libraries(arm_face_identify PRIVATE atomic)
#target_link_libraries(arm_face_identify PRIVATE boost_filesystem)

#cmake -DCMAKE_INSTALL_PREFIX=/usr/local/arm-face-identify  ..
# 在执行make install 的时候会把data目录下的文件复制到 CMAKE_INSTALL_PREFIX 目录下的data目录
install(DIRECTORY data/ DESTINATION data)


#树莓派
# cmake -DCMAKE_INSTALL_PREFIX=/usr/local/arm-face-identify -DCMAKE_CXX_STANDARD=11 -DCMAKE_SHARED_LINKER_FLAGS='-latomic'  ..