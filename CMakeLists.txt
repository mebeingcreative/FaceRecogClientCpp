cmake_minimum_required(VERSION 3.8)
project(FaceRecogClientCpp CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS
        Widgets
        Network
        WebEngineWidgets
        REQUIRED)

add_subdirectory(libs/dlib)

add_executable(FaceRecogClientCpp
        src/main.cpp
        src/face_recog_api/face_recog_api.cpp
        src/face_recog_api/face_recog_service.cpp
        src/face_detect/face_detector.cpp
        src/face_recog_demo.cpp
        src/qtglwebcamdemo.ui
        src/qviewergl.cpp
        )

target_link_libraries(FaceRecogClientCpp
        PRIVATE
        ${OpenCV_LIBS}
        dlib::dlib
        Qt5::Widgets
        Qt5::Network
        Qt5::WebEngineWidgets
        )

target_include_directories(FaceRecogClientCpp
        PRIVATE
            src
            OpenCV::OpenCV
        )

set(LANDMARK_FILE "${CMAKE_SOURCE_DIR}/res/shape_predictor_68_face_landmarks.dat")

configure_file(${LANDMARK_FILE} shape_predictor_68_face_landmarks.dat COPYONLY)
if (NOT EXISTS ${LANDMARK_FILE})
    message(SEND_ERROR "You need dlib's default face landmarking model in order to run.\
            http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2 extract it to ${LANDMARK_FILE}"
            )
endif()
