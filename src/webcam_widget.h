#ifndef QVIEWERGL_H
#define QVIEWERGL_H

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <opencv2/opencv.hpp>

#include "face_detect/face_detector.h"
#include "face_recog_api/face_recog_service.h"

class webcam_widget :
        public QWidget {
Q_OBJECT
public:
    explicit webcam_widget(QWidget* parent = 0);
    ~webcam_widget() override;
protected:
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;
private:
    double detectionScale;
    cv::VideoCapture capture;
    face_detector detector;
    cv::Mat bgrImage;
    cv::Mat resizedImage;
    cv::Mat rgbaImage;
    QImage qimage;
    QTimer* timer;
    QSize cameraSize;
    cv::Size detectionSize;
    face_recog_service recog_service;
};

#endif // CQTOPENCVVIEWERGL_H
