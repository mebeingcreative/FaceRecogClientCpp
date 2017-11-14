#include "qviewergl.h"
#include <QDebug>

QViewerGl::QViewerGl(QWidget* parent) :
        recog_service{},
        QWidget{parent}
{
    if (!capture.isOpened() && !capture.open(0)) {
        qCritical() << "Failed to open camera";
        return;
    }

    cameraSize = QSize{
            static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH)),
            static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH))
    };

    detector.initialize();

    timer = new QTimer{this};
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
}

QViewerGl::~QViewerGl() = default;

void QViewerGl::paintEvent(QPaintEvent * event) {
    capture.read(imageBGR);
    cvtColor(imageBGR, imageRGBA, CV_BGR2RGBA);

    QPainter painter{this};
    auto const origin = QPoint{0,0};
    qimage = QImage{imageRGBA.data, imageRGBA.cols, imageRGBA.rows, QImage::Format_RGBA8888_Premultiplied};
    painter.drawImage(origin, qimage);

    auto const faces = detector.detect(imageBGR);
    for (auto const & f: faces){
        painter.drawRect(f.x, f.y, f.width, f.height);
    }
    if (!faces.empty()) {
        recog_service.recognize(imageBGR, faces);
    }
}

QSize QViewerGl::minimumSizeHint() const {
    return cameraSize;
}