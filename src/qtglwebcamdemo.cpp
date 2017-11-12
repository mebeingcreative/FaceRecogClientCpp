#include <QDebug>

#include "qtglwebcamdemo.h"
#include "ui_qtglwebcamdemo.h"

QtGLWebcamDemo::QtGLWebcamDemo(QWidget* parent) :
        api{"http://localhost:5000"},
        QMainWindow(parent) {
    ui = std::make_unique<Ui::QtGLWebcamDemo>();
    ui->setupUi(this);
    ui->webView->load(QUrl("https://face.otep.ch/locations/2/current"));
    ui->webView->show();
}

QtGLWebcamDemo::~QtGLWebcamDemo() = default;
