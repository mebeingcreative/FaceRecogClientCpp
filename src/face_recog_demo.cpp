#include <QDebug>

#include "face_recog_demo.h"
#include "ui_face_recog_demo.h"
#include "face_recog_config.h"

face_recog_demo::face_recog_demo(QWidget* parent):
        QMainWindow(parent){
    auto config = fetch_config();
    ui = std::make_unique<Ui::face_recog_demo>();
    ui->setupUi(this);
    ui->webView->load(config.tracking_view_url);
    ui->webView->show();
}

face_recog_demo::~face_recog_demo() = default;
