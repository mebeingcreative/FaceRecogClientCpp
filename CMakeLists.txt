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

set(LANDMARK_FILE "${CMAKE_SOURCE_DIR}/res/shape_predictor_68_face_landmarks.dat")

if (NOT EXISTS ${LANDMARK_FILE})
    message(SEND_ERROR "You need dlib's default face landmarking model in order to run.\
            http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2 extract it to ${LANDMARK_FILE}"
            )
endif()