cmake_minimum_required(VERSION 3.8)
project(face-recog-client CXX)

set(CMAKE_CXX_STANDARD 14)

# Include Dlib

set(DLIB_NO_GUI_SUPPORT true)
set(DLIB_JPEG_SUPPORT false)
set(DLIB_LINK_WITH_SQLITE3 false)
set(DLIB_USE_BLAS false)
set(DLIB_USE_LAPACK false)
set(DLIB_USE_CUDA false)
set(DLIB_PNG_SUPPORT false)
set(DLIB_GIF_SUPPORT false)
set(DLIB_USE_MKL_FFT false)
add_subdirectory(libs/dlib)

# Include OpenCV 2

find_package(OpenCV COMPONENTS
        core
        imgproc
        highgui
        REQUIRED)

# Include Qt5

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Widgets
        Network
        WebEngineWidgets
        REQUIRED)

# Project Definitions

add_executable(face-recog-client
        src/main.cpp
        src/face_recog_api/face_recog_api.cpp
        src/face_recog_api/face_recog_service.cpp
        src/face_detect/face_detector.cpp
        src/face_recog_demo.cpp
        src/webcam_widget.cpp
        )


target_link_libraries(face-recog-client
        PRIVATE
            ${OpenCV_LIBS}
            dlib::dlib
            Qt5::Widgets
            Qt5::Network
            Qt5::WebEngineWidgets
        )

target_include_directories(face-recog-client
        PRIVATE
            src
            OpenCV::OpenCV
        )

# Runtime Files

set(FACE_RECOG_LANDMARK_PATH "${CMAKE_SOURCE_DIR}/res/shape_predictor_68_face_landmarks.dat")
if (NOT EXISTS ${FACE_RECOG_LANDMARK_PATH})
    message(SEND_ERROR "You need dlib's default face landmarking model in order to run.\
            http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2 extract it to ${FACE_RECOG_LANDMARK_PATH}"
            )
endif()
configure_file(res/face-recog-config.ini.in face-recog-config.ini)

# == Install ==
include(GNUInstallDirs)

set(FACE_RECOG_CONFIG_PATH "${CMAKE_INSTALL_FULL_SYSCONFDIR}/face-recog-config.ini")
set(FACE_RECOG_DATA_PATH "${CMAKE_INSTALL_FULL_DATADIR}/face-recog-client")
set(FACE_RECOG_LANDMARK_PATH "${FACE_RECOG_DATA_PATH}/shape_predictor_68_face_landmarks.dat")

configure_file(res/face-recog-config.ini.in face-recog-config-release.ini)
configure_file(res/face-recog-client-bootstrapper.sh.in face-recog-client-bootstrapper.sh)

install(TARGETS face-recog-client
        DESTINATION ${FACE_RECOG_DATA_PATH})
install(FILES res/shape_predictor_68_face_landmarks.dat
        DESTINATION ${FACE_RECOG_DATA_PATH})
install(FILES "${CMAKE_BINARY_DIR}/face-recog-config-release.ini"
        DESTINATION "${CMAKE_INSTALL_FULL_SYSCONFDIR}"
        RENAME "face-recog-config.ini")
install(FILES "${CMAKE_BINARY_DIR}/face-recog-client-bootstrapper.sh"
        DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        RENAME "face-recog-client")

# == CPack ==

set(CPACK_PACKAGE_DESCRIPTION "A face recognition showcase from the University of Applied Sciences Rapperswil")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A face recognition showcase from HSR")
set(CPACK_PACKAGE_CONTACT "Victor Ruch")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/release")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/desc.txt")

# --- Package config ---
set(CPACK_GENERATOR "DEB")
set(CPACK_MONOLITHIC_INSTALL 1)
set(CPACK_STRIP_FILES 1)
#set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")

# --- Source package config ---
set(CPACK_SOURCE_GENERATOR "DEB")

# --- DEB package config ---
set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.15), libconfig9, libdbus-1-3 (>= 1.1.1), libgl1-mesa-glx | libgl1 | libgl1-nvidia-glx | libgl1-fglrx-glx, libpcre3 (>= 8.10), libx11-6, libxcomposite1 (>= 1:0.3-1), libxdamage1 (>= 1:1.1), libxext6, libxfixes3, libxrandr2 (>= 4.3), libxrender1, libxinerama1")

include(CPack)