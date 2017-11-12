#include "qviewergl.h"
#include <QDebug>

void convert_to_jpeg(cv::Mat & mat, std::vector<unsigned char> & out) {
    std::vector<int> params{ cv::IMWRITE_JPEG_QUALITY, 80 };
    out.clear();
    cv::imencode(".jpg", mat, out, params);
    //std::ofstream outfile ("test.jpg", std::ofstream::binary);
    //outfile.write(reinterpret_cast<char *>(out.data()), out.size());
}

QByteArray convert_to_qbytearray(std::vector<unsigned char> const & vector){
    return {reinterpret_cast<char const *>(vector.data()), static_cast<int const>(vector.size())};
}

QViewerGl::QViewerGl(QWidget* parent) :
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

    detector.detect(imageBGR);

    QPainter painter{this};
    auto const origin = QPoint{0,0};
    cvtColor(imageBGR, imageRGBA, CV_BGR2RGBA);
    qimage = QImage{imageRGBA.data, imageRGBA.cols, imageRGBA.rows, QImage::Format_RGBA8888_Premultiplied};
    painter.drawImage(origin, qimage);
}

QSize QViewerGl::minimumSizeHint() const {
    return cameraSize;
}