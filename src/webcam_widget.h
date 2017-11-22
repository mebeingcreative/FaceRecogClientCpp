#ifndef QVIEWERGL_H
#define QVIEWERGL_H

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <opencv2/opencv.hpp>

#include "face_detect/face_detector.h"
#include "face_recog_api/face_recog_service.h"
#include "webcam.h"

class webcam_widget :
        public QWidget {
Q_OBJECT
public:
    explicit webcam_widget(QWidget* parent = nullptr);
    ~webcam_widget() override;
protected:
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;
private:
    QImage wrap_mat(cv::Mat const & image);
    webcam camera;
    cv::Mat rgbaImage;
    QTimer* timer;
};

#endif // CQTOPENCVVIEWERGL_H
