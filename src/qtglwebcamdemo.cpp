#include <QDebug>

#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include "qtglwebcamdemo.h"
#include "ui_qtglwebcamdemo.h"

QtGLWebcamDemo::QtGLWebcamDemo(QWidget* parent) :
        api{"http://localhost:5000"},
        QMainWindow(parent) {
    ui = std::make_unique<Ui::QtGLWebcamDemo>();
    ui->setupUi(this);
}

QtGLWebcamDemo::~QtGLWebcamDemo() = default;
