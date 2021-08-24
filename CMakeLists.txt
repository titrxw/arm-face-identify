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
LINK_DIRECTORIES(${OpenCV_LIB_DIR})

# Declare the executable target built from your sources
add_executable(arm_face_identify
        main.cpp identify/Source/FaceIdentify.cpp identify/Include/FaceIdentify.h identify/Include/Identify.h identify/Source/Identify.cpp train/Source/FaceTrain.cpp train/Include/FaceTrain.h train/Include/Train.h train/Source/Train.cpp helper/Include/Str.h helper/Source/Str.cpp train/Source/VideoFaceTrain.cpp train/Include/VideoFaceTrain.h Common/Source/Opencv.cpp Common/Include/Opencv.h helper/Source/File.cpp helper/Include/File.h)

# Link your application with OpenCV libraries
target_link_libraries(arm_face_identify PRIVATE ${OpenCV_LIBS})