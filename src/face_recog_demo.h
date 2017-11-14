#ifndef FACERECOGCLIENTCPP_FACE_RECOG_DEMO_H
#define FACERECOGCLIENTCPP_FACE_RECOG_DEMO_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include "face_recog_api/face_recog_api.h"


namespace Ui {
    class QtGLWebcamDemo;
}

class face_recog_demo:
        public QMainWindow {
Q_OBJECT

public:
    explicit face_recog_demo(QWidget* parent = 0);
    ~face_recog_demo() override;

private:
    std::unique_ptr<Ui::QtGLWebcamDemo> ui;
};

#endif // FACERECOGCLIENTCPP_FACE_RECOG_DEMO_H
