//
// Created by viruch on 22.11.17.
//

#include "webcam.h"

webcam::webcam():
    capture{},
    detector{},
    recog_service{},
    size{}
{
    if (!capture.isOpened() && !capture.open(0)) {
        throw std::runtime_error{"Failed to open camera! Check if camera is connected"};
    }

    size.width = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
    size.height = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));
}

void webcam::read(cv::Mat & mat){
    capture.read(mat);
}

void webcam::process_frame(){

}