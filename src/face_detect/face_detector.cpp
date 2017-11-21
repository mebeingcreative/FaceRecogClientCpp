//
// Created by viruch on 07.11.17.
//

#include <QDebug>

#include "face_detector.h"
#include "face_recog_config.h"

dlib::rectangle & grow_margin(dlib::rectangle & r) {
    auto margin = 44.0f / 94.0f * r.width();
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

std::vector<cv::Rect> face_detector::detect(cv::Mat & image){
    // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
    // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
    // long as temp is valid.  Also don't do anything to temp that would cause it
    // to reallocate the memory which stores the image as that will make cimg
    // contain dangling pointers.  This basically means you shouldn't modify temp
    // while using cimg.
    dlib::cv_image<dlib::bgr_pixel> cimg(image);

    auto shapes = std::vector<dlib::full_object_detection>{};
    auto rects = std::vector<cv::Rect>{};
    std::vector<dlib::rectangle> faces = detector(cimg);
    for (auto & face : faces) {
        shapes.push_back(pose_model(cimg, face));
        rects.push_back(transform_to_rect(image, grow_margin(face)));
    }

    /*dlib::array<dlib::array2d<dlib::rgb_pixel>> face_chips;
    extract_image_chips(cimg, get_face_chip_details(shapes), face_chips);
    face_win.set_image(dlib::tile_images(face_chips));*/

    return rects;
}

cv::Rect face_detector::transform_to_rect(cv::Mat const & mat, dlib::rectangle const & r) {
    return {
            cv::Point2i(
                    static_cast<int>(std::max(0l, r.left()) / scalingFactor),
                    static_cast<int>(std::max(0l, r.top()) / scalingFactor)),
            cv::Point2i(
                    static_cast<int>(std::min(mat.cols - 1l, r.right() + 1) / scalingFactor),
                    static_cast<int>(std::min(mat.rows - 1l, r.bottom()  + 1) / scalingFactor))
    };
}