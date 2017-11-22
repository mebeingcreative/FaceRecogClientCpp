//
// Created by viruch on 07.11.17.
//

#ifndef FACERECOGCLIENTCPP_FACE_DETECTOR_H
#define FACERECOGCLIENTCPP_FACE_DETECTOR_H

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/shape_predictor.h>

struct face_detector{
    explicit face_detector(double scalingFactor = 0.75);
    std::vector<cv::Rect> detect(cv::Mat const & originalMat);
private:
    cv::Rect transform_to_rect(cv::Size const & maxSize, dlib::rectangle const & r) const;

    double const scalingFactor;
    dlib::frontal_face_detector detector{};
    dlib::shape_predictor pose_model{};
    cv::Mat scaledMat;
};

#endif //FACERECOGCLIENTCPP_FACE_DETECTOR_H
