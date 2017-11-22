//
// Created by viruch on 22.11.17.
//

#ifndef FACE_RECOG_CLIENT_WEBCAM_H
#define FACE_RECOG_CLIENT_WEBCAM_H

#include <opencv2/opencv.hpp>
#include "face_detect/face_detector.h"
#include "face_recog_api/face_recog_service.h"

struct webcam {
    webcam();
    void read(cv::Mat & mat);
    void process_frame();
    cv::Size size;
private:
    cv::VideoCapture capture;
    cv::Mat bgrImage;
    face_detector detector;
    face_recog_service recog_service;
    cv::Mat rgbaImage;
};


#endif //FACE_RECOG_CLIENT_WEBCAM_H
