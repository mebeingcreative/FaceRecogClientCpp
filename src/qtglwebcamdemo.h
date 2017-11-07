#ifndef QTGLWEBCAMDEMO_H
#define QTGLWEBCAMDEMO_H

#include <QMainWindow>
#include <memory>
#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include "face_recog_api/face_recog_api.h"


namespace Ui {
    class QtGLWebcamDemo;
}

class QtGLWebcamDemo : public QMainWindow {
Q_OBJECT

public:
    explicit QtGLWebcamDemo(QWidget* parent = 0);
    ~QtGLWebcamDemo() override;

protected:
    void timerEvent(QTimerEvent* event) override;

private slots:
    void on_actionStart_triggered();

private:
    std::unique_ptr<Ui::QtGLWebcamDemo> ui;
    cv::VideoCapture capture{};
    dlib::frontal_face_detector detector{};
    dlib::shape_predictor pose_model{};
    face_recog_api api;
};

#endif // QTGLWEBCAMDEMO_H
