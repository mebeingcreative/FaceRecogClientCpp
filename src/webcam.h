//
// Created by viruch on 22.11.17.
//

#ifndef FACE_RECOG_CLIENT_WEBCAM_H
#define FACE_RECOG_CLIENT_WEBCAM_H

#include <vector>
#include <opencv2/opencv.hpp>
#include "face_detect/face_detector.h"
#include "face_recog_api/face_recog_service.h"

struct webcam {
    webcam();
    void update();
    cv::Size size;
    cv::Mat image;
    std::vector<cv::Rect> faces;
private:
    cv::VideoCapture capture;
    face_detector detector;
    face_recog_service recog_service;
};


#endif //FACE_RECOG_CLIENT_WEBCAM_H
