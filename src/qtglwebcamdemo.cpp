#include <QDebug>

#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include "qtglwebcamdemo.h"
#include "ui_qtglwebcamdemo.h"


cv::Rect dlibRectangleToOpenCV(dlib::rectangle const & r) {
    return {cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1)};
}

void convert_to_jpeg(cv::Mat & mat, std::vector<unsigned char> & out) {
    std::vector<int> params{ cv::IMWRITE_JPEG_QUALITY, 80 };
    out.clear();
    cv::imencode(".jpg", mat, out, params);
    std::ofstream outfile ("test.jpg", std::ofstream::binary);
    outfile.write(reinterpret_cast<char *>(out.data()), out.size());
}

template<int margins = 44L>
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


    // Detect faces
    std::vector<dlib::rectangle> faces = detector(cimg);
    // Find the pose of each face.
    auto shapes = std::vector<dlib::full_object_detection>{};
    auto jpgImage = std::vector<unsigned char>{};
    for (auto & face : faces) {
        shapes.push_back(pose_model(cimg, face));
//                cv::Mat face_mat;
//                originalImage.copyTo(face_mat(dlibRectangleToOpenCV(face)));

        grow_margin(face);
        convert_to_jpeg(image, jpgImage);
        auto qarray = QByteArray(reinterpret_cast<char *>(jpgImage.data()), jpgImage.size());
        api.request_embedding(qarray, face);
    }

    /*dlib::array<dlib::array2d<dlib::rgb_pixel>> face_chips;
    extract_image_chips(cimg, get_face_chip_details(shapes), face_chips);
    face_win.set_image(dlib::tile_images(face_chips));*/

    ui->openCVviewer->showImage(image);
}
