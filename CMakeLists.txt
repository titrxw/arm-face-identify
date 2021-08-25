cmake_minimum_required(VERSION 3.17)
set(CMAKE_BUILD_TYPE Debug)

# Define project name
project(arm_face_identify)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

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
INCLUDE_DIRECTORIES("./vendor")
LINK_DIRECTORIES(${OpenCV_LIB_DIR})

# Declare the executable target built from your sources
add_executable(arm_face_identify
        main.cpp core/identify/Source/FaceIdentify.cpp core/identify/Include/FaceIdentify.h core/identify/Include/Identify.h core/identify/Source/Identify.cpp core/train/Source/FaceTrain.cpp core/train/Include/FaceTrain.h core/train/Include/Train.h core/train/Source/Train.cpp core/helper/Include/Str.h core/helper/Source/Str.cpp business/train/Source/VideoFaceTrain.cpp business/train/Include/VideoFaceTrain.h core/base/Source/Opencv.cpp core/base/Include/Opencv.h core/helper/Source/File.cpp core/helper/Include/File.h core/base/Enum/Event.h)

# Link your application with OpenCV libraries
target_link_libraries(arm_face_identify PRIVATE ${OpenCV_LIBS})