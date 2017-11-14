#include <QDebug>

#include "face_recog_demo.h"
#include "ui_face_recog_demo.h"

face_recog_demo::face_recog_demo(config const & cfg, QWidget* parent):
        QMainWindow(parent){
    ui = std::make_unique<Ui::face_recog_demo>();
    ui->setupUi(this);
    ui->webView->load(QUrl("https://face.otep.ch/locations/1/current"));
    ui->webView->show();
}

face_recog_demo::~face_recog_demo() = default;
