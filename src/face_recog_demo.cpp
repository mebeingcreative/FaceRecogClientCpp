#include <QDebug>

#include "face_recog_demo.h"
#include "face_recog_config.h"
#include "ui_face_recog_demo.h"

face_recog_demo::face_recog_demo(QWidget* parent):
        QMainWindow(parent){
    auto config = fetch_config();
    ui = std::make_unique<Ui::face_recog_demo>();
    ui->setupUi(this);
    auto web_view = ui->webView;
    web_view->load(config.tracking_view_url);
    web_view->show();
}

face_recog_demo::~face_recog_demo() = default;
