#ifndef QVIEWERGL_H
#define QVIEWERGL_H

#include <QtGui>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

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
    void detect(cv::Mat & image);
    cv::VideoCapture capture{};
    dlib::frontal_face_detector detector{};
    dlib::shape_predictor pose_model{};
    cv::Mat imageBGR;
    cv::Mat imageRGBA;
    QImage qimage;
    QTimer* timer;
    QSize cameraSize;
};

#endif // CQTOPENCVVIEWERGL_H
