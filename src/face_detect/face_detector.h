//
// Created by viruch on 07.11.17.
//

#ifndef FACERECOGCLIENTCPP_FACE_DETECTOR_H
#define FACERECOGCLIENTCPP_FACE_DETECTOR_H

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/shape_predictor.h>
#include <QVector>
#include <QRect>

struct face_detector{
    face_detector(double scalingFactor = 0.25);
    std::vector<cv::Rect> detect(cv::Mat & image);
private:
    double const scalingFactor;
    dlib::frontal_face_detector detector{};
    dlib::shape_predictor pose_model{};
    cv::Rect transform_to_rect(cv::Mat const & mat, dlib::rectangle const & r);
};

#endif //FACERECOGCLIENTCPP_FACE_DETECTOR_H
