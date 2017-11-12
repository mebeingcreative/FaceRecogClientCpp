//
// Created by viruch on 07.11.17.
//

#ifndef FACERECOGCLIENTCPP_FACE_DETECTOR_H
#define FACERECOGCLIENTCPP_FACE_DETECTOR_H

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing/shape_predictor.h>
#include <QVector>
#include <QRect>

struct face_detector{
    void initialize();
    QVector<QRect> detect(cv::Mat & image);
private:
    dlib::frontal_face_detector detector{};
    dlib::shape_predictor pose_model{};
};

#endif //FACERECOGCLIENTCPP_FACE_DETECTOR_H
