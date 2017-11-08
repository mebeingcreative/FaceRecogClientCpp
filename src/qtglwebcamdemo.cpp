#include <QDebug>

#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include "qtglwebcamdemo.h"
#include "ui_qtglwebcamdemo.h"


cv::Mat get_roi_from_rectangle(cv::Mat const & mat, dlib::rectangle const & r) {
    auto const roi = cv::Rect{
            cv::Point2i(
                    std::max(0, static_cast<int>(r.left())),
                    std::max(0, static_cast<int>(r.top()))),
            cv::Point2i(
                    std::min(mat.cols - 1, static_cast<int>(r.right() + 1)),
                    std::min(mat.rows - 1 , static_cast<int>(r.bottom() + 1)))
    };
    return mat(roi);
}

void convert_to_jpeg(cv::Mat & mat, std::vector<unsigned char> & out) {
    std::vector<int> params{ cv::IMWRITE_JPEG_QUALITY, 80 };
    out.clear();
    cv::imencode(".jpg", mat, out, params);
    //std::ofstream outfile ("test.jpg", std::ofstream::binary);
    //outfile.write(reinterpret_cast<char *>(out.data()), out.size());
}

QByteArray convert_to_qbytearray(std::vector<unsigned char> const & vector){
    return {reinterpret_cast<char const *>(vector.data()), static_cast<int const>(vector.size())};
}

template<int margins = 0L>
void grow_margin(dlib::rectangle & r) {
    r.left() -= margins;
    r.top() -= margins;
    r.right() += 2 * margins;
    r.bottom() += 2 * margins;
}


QtGLWebcamDemo::QtGLWebcamDemo(QWidget* parent) :
        api{"http://localhost:5000"},
        QMainWindow(parent) {
    ui = std::make_unique<Ui::QtGLWebcamDemo>();
    ui->setupUi(this);
    on_actionStart_triggered();
}

QtGLWebcamDemo::~QtGLWebcamDemo() = default;

void QtGLWebcamDemo::on_actionStart_triggered() {
    if (!capture.isOpened() && !capture.open(0)) {
        qCritical() << "Failed to open camera";
        return;
    }

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

    startTimer(0);
}

void QtGLWebcamDemo::timerEvent(QTimerEvent*) {
    cv::Mat image;
    capture.read(image);

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
        auto face_roi = get_roi_from_rectangle(image, face);
        convert_to_jpeg(face_roi, jpgImage);
    }

    auto qarray = convert_to_qbytearray(jpgImage);
    //api.request_embedding(qarray, face);

    /*dlib::array<dlib::array2d<dlib::rgb_pixel>> face_chips;
    extract_image_chips(cimg, get_face_chip_details(shapes), face_chips);
    face_win.set_image(dlib::tile_images(face_chips));*/

    ui->openCVviewer->showImage(image);
}
