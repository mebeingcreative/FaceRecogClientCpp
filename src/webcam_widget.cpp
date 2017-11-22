#include "webcam_widget.h"
#include <QDebug>
#include <QPainter>
#include <stdexcept>

webcam_widget::webcam_widget(QWidget* parent) :
        QWidget{parent},
        capture{},
        detector{},
        recog_service{}
{
    if (!capture.isOpened() && !capture.open(0)) {
        throw std::runtime_error{"Failed to open camera! Check if camera is connected"};
    }

    auto cameraWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    auto cameraHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    cameraSize = QSize{
            static_cast<int>(cameraWidth),
            static_cast<int>(cameraHeight)
    };

    timer = new QTimer{this};
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);
}

webcam_widget::~webcam_widget() = default;

void webcam_widget::paintEvent(QPaintEvent * event) {
    capture.read(bgrImage);
    cvtColor(bgrImage, rgbaImage, CV_BGR2RGBA);

    QPainter painter{this};
    painter.setPen(Qt::green);
    auto const origin = QPoint{0,0};
    qimage = QImage{rgbaImage.data, rgbaImage.cols, rgbaImage.rows, QImage::Format_RGBA8888_Premultiplied};
    painter.drawImage(origin, qimage);

    auto const faces = detector.detect(bgrImage);
    for (auto const & f: faces){
        painter.drawRect(f.x, f.y, f.width, f.height);
    }
    if (!faces.empty()) {
        recog_service.recognize(bgrImage, faces);
    }
}

QSize webcam_widget::minimumSizeHint() const {
    return cameraSize;
}
