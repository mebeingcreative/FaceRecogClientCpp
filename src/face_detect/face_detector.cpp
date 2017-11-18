//
// Created by viruch on 07.11.17.
//

#include <QDebug>

#include "face_detector.h"
#include "face_recog_config.h"

inline cv::Rect transform_to_rect(cv::Mat const & mat, dlib::rectangle const & r) {
    return {
            cv::Point2i(
                    std::max(0, static_cast<int>(r.left())),
                    std::max(0, static_cast<int>(r.top()))),
            cv::Point2i(
                    std::min(mat.cols - 1, static_cast<int>(r.right() + 1)),
                    std::min(mat.rows - 1 , static_cast<int>(r.bottom() + 1)))
    };
}

inline QRect transform_to_qrect(dlib::rectangle const & r){
    return {static_cast<int>(r.left()), static_cast<int>(r.top()),
            static_cast<int>(r.width()), static_cast<int>(r.height())};
}

template<int margins = 0L>
void grow_margin(dlib::rectangle & r) {
    r.left() -= margins;
    r.top() -= margins;
    r.right() += 2 * margins;
    r.bottom() += 2 * margins;
}

face_detector::face_detector() {
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
        rects.push_back(transform_to_rect(image, face));
    }

    /*dlib::array<dlib::array2d<dlib::rgb_pixel>> face_chips;
    extract_image_chips(cimg, get_face_chip_details(shapes), face_chips);
    face_win.set_image(dlib::tile_images(face_chips));*/

    return rects;
}