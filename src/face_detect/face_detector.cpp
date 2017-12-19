//
// Created by viruch on 07.11.17.
//

#include <QDebug>
#include <opencv2/opencv.hpp>

#include "face_detector.h"
#include "face_recog_config.h"

dlib::rectangle & grow_margin(dlib::rectangle & r) {
    auto margin = 32.0f / (160.0f/2.0f) * r.width();
    r.top() -= margin;
    r.left() -= margin;
    r.right() += margin;
    r.bottom() += margin;
    return r;
}

face_detector::face_detector(double scalingFactor):
    scalingFactor{scalingFactor}
{
    auto config = fetch_config();
    try {
        detector = dlib::get_frontal_face_detector();
        dlib::shape_predictor pose_model;
        dlib::deserialize(config.predictor_path) >> pose_model;
    } catch (dlib::serialization_error & e) {
        qCritical() << "dlib's default face landmarking model is missing!" << e.what() << "\n";
        throw e;
    }
}

std::vector<cv::Rect> face_detector::detect(cv::Mat const & originalMat){
    auto originalSize = originalMat.size();
    auto detectionSize = cv::Size{
            static_cast<int>(originalSize.width * scalingFactor),
            static_cast<int>(originalSize.height * scalingFactor)};
    cv::resize(originalMat, scaledMat, detectionSize, 0, 0, cv::INTER_AREA);
    dlib::cv_image<dlib::bgr_pixel> cimg(scaledMat);

    auto shapes = std::vector<dlib::full_object_detection>{};
    auto rects = std::vector<cv::Rect>{};
    std::vector<dlib::rectangle> faces = detector(cimg);
    for (auto & face : faces) {
        shapes.push_back(pose_model(cimg, face));
        rects.push_back(transform_to_rect(originalSize, grow_margin(face)));
    }

    /*dlib::array<dlib::array2d<dlib::rgb_pixel>> face_chips;
    extract_image_chips(cimg, get_face_chip_details(shapes), face_chips);
    face_win.set_image(dlib::tile_images(face_chips));*/

    return rects;
}

cv::Rect face_detector::transform_to_rect(cv::Size const & maxSize, dlib::rectangle const & r) const {
    auto matRect = cv::Rect(cv::Point(0, 0), cv::Point(maxSize.width - 1, maxSize.height -1));
    auto rescaledRect = cv::Rect{
            cv::Point(
                    static_cast<int>(r.left() / scalingFactor),
                    static_cast<int>(r.top() / scalingFactor)
            ),
            cv::Point(
                    static_cast<int>((r.right() + 1) / scalingFactor),
                    static_cast<int>((r.bottom()  + 1) / scalingFactor)
            )
    };
    return matRect & rescaledRect;
}