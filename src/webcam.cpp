//
// Created by viruch on 22.11.17.
//

#include "webcam.h"

webcam::webcam():
        size{},
        capture{},
        detector{},
        recog_service{}
{
    if (!capture.isOpened() && !capture.open(0)) {
        throw std::runtime_error{"Failed to open camera! Check if camera is connected"};
    }

    size.width = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
    size.height = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));
}

void webcam::update(){
    capture.read(image);
    faces = detector.detect(image);
    if (!faces.empty()) {
        recog_service.recognize(image, faces);
    }
}