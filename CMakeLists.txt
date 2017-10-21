cmake_minimum_required(VERSION 3.8)
project(FaceRecogClientCpp)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIR})

include_directories(src)

add_subdirectory(libs/dlib)

add_executable(FaceRecogClientCpp
        src/webcam_face_pose_ex.cpp
        )
target_link_libraries(FaceRecogClientCpp
        ${OpenCV_LIBS}
        dlib::dlib
        )
