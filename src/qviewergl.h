#ifndef QVIEWERGL_H
#define QVIEWERGL_H

#include <QtGui>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "face_detect/face_detector.h"

class QViewerGl :
        public QWidget {
Q_OBJECT
public:
    explicit QViewerGl(QWidget* parent = 0);
    ~QViewerGl() override;
protected:
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;
private:
    cv::VideoCapture capture{};
    face_detector detector{};
    cv::Mat imageBGR;
    cv::Mat imageRGBA;
    QImage qimage;
    QTimer* timer;
    QSize cameraSize;
};

#endif // CQTOPENCVVIEWERGL_H
