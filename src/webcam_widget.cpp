#include "webcam_widget.h"
#include <QDebug>
#include <QPainter>
#include <stdexcept>

webcam_widget::webcam_widget(QWidget* parent) :
        QWidget{parent},
        camera{}
{
    timer = new QTimer{this};
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);
}

webcam_widget::~webcam_widget() = default;

void webcam_widget::paintEvent(QPaintEvent * event) {
    camera.update();

    QPainter painter{this};
    painter.setPen(Qt::green);
    painter.drawImage(QPoint{0,0}, wrap_mat(camera.image));

    for (auto const & f: camera.faces){
        painter.drawRect(f.x, f.y, f.width, f.height);
    }
}

QSize webcam_widget::minimumSizeHint() const {
    return {camera.size.width, camera.size.height};
}

QImage webcam_widget::wrap_mat(cv::Mat const & image){
    cvtColor(image, rgbaImage, CV_BGR2RGBA);
    return {rgbaImage.data, rgbaImage.cols, rgbaImage.rows, QImage::Format_RGBA8888_Premultiplied};
}
