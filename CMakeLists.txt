cmake_minimum_required(VERSION 3.8)
project(FaceRecogClientCpp CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

find_package(OpenCV REQUIRED)

add_subdirectory(libs/dlib)

add_executable(FaceRecogClientCpp
        src/main.cpp
        )

target_link_libraries(FaceRecogClientCpp
        ${OpenCV_LIBS}
        dlib::dlib
        )

target_include_directories(FaceRecogClientCpp
        PRIVATE
            src
            OpenCV::OpenCV
        )

set(LANDMARK_FILE "${CMAKE_SOURCE_DIR}/res/shape_predictor_68_face_landmarks.dat")

if (NOT EXISTS ${LANDMARK_FILE})
    message(SEND_ERROR "You need dlib's default face landmarking model in order to run.\
            http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2 extract it to ${LANDMARK_FILE}"
            )
endif()