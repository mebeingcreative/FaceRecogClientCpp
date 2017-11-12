//
// Created by viruch on 07.11.17.
//

#include "face_detector.h"
#include <QDebug>
#include <algorithm>
#include <iterator>

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

void face_detector::initialize() {
    try {
        detector = dlib::get_frontal_face_detector();
        dlib::shape_predictor pose_model;
        dlib::deserialize("../res/shape_predictor_68_face_landmarks.dat") >> pose_model;
    } catch (dlib::serialization_error & e) {
        qCritical() << "You need dlib's default face landmarking model file to run this example.\n"
                "You can get it from the following URL: \n"
                "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2\n\n"
                    << e.what() << "\n";
    }
}

QVector<QRect> face_detector::detect(cv::Mat & image){
    // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
    // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
    // long as temp is valid.  Also don't do anything to temp that would cause it
    // to reallocate the memory which stores the image as that will make cimg
    // contain dangling pointers.  This basically means you shouldn't modify temp
    // while using cimg.
    dlib::cv_image<dlib::bgr_pixel> cimg(image);

    std::vector<dlib::rectangle> faces = detector(cimg);

    auto shapes = std::vector<dlib::full_object_detection>{};
    auto jpgImage = std::vector<unsigned char>{};

    for (auto & face : faces) {
        shapes.push_back(pose_model(cimg, face));
//                cv::Mat face_mat;
//                originalImage.copyTo(face_mat(dlibRectangleToOpenCV(face)));

        grow_margin(face);
        auto face_rect = transform_to_rect(image, face);
      //  convert_to_jpeg(image(face_rect), jpgImage);
    }

    //auto qarray = convert_to_qbytearray(jpgImage);
    //api.request_embedding(qarray, face);

    /*dlib::array<dlib::array2d<dlib::rgb_pixel>> face_chips;
    extract_image_chips(cimg, get_face_chip_details(shapes), face_chips);
    face_win.set_image(dlib::tile_images(face_chips));*/

    QVector<QRect> out;
    std::transform(faces.cbegin(), faces.cend(), std::back_inserter(out), transform_to_qrect);
    return out;
}