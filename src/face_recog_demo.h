#ifndef FACERECOGCLIENTCPP_FACE_RECOG_DEMO_H
#define FACERECOGCLIENTCPP_FACE_RECOG_DEMO_H

#include <QMainWindow>
#include <memory>
#include "config.h"

namespace Ui {
    class face_recog_demo;
}

class face_recog_demo:
        public QMainWindow {
Q_OBJECT

public:
    face_recog_demo(config const & cfg, QWidget* parent = 0);
    ~face_recog_demo() override;

private:
    std::unique_ptr<Ui::face_recog_demo> ui;
};

#endif // FACERECOGCLIENTCPP_FACE_RECOG_DEMO_H
