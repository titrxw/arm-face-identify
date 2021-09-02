#cmake_minimum_required(VERSION 3.17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(projectName arm_face_identify)

# Define project name
project(${projectName})

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
add_executable(${projectName}
        main.cpp )

# Link your application with OpenCV libraries
target_link_libraries(${projectName} PRIVATE ${OpenCV_LIBS})

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^(arm.*|ARM.*)")
    target_link_libraries(${projectName} PRIVATE atomic)
endif()

# target_link_libraries(${projectName} PRIVATE boost_filesystem)

# cmake -DCMAKE_INSTALL_PREFIX=/usr/local/arm-face-identify  ..
# 在执行make install 的时候会把data目录下的文件复制到 CMAKE_INSTALL_PREFIX 目录下的data目录
install(DIRECTORY data/ DESTINATION data)


#树莓派
# cmake -DCMAKE_INSTALL_PREFIX=/usr/local/arm-face-identify -DCMAKE_CXX_STANDARD=11 -DCMAKE_SHARED_LINKER_FLAGS='-latomic'  ..