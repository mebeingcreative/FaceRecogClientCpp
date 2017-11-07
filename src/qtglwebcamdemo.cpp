#include <QDebug>
#include "qtglwebcamdemo.h"
#include "ui_qtglwebcamdemo.h"

QtGLWebcamDemo::QtGLWebcamDemo(QWidget* parent) :
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
    startTimer(0);
}

void QtGLWebcamDemo::timerEvent(QTimerEvent*) {
    cv::Mat image;
    capture.read(image);

    // Do what you want with the image :-)

    ui->openCVviewer->showImage(image);
}
