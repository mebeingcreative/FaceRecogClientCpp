#ifndef QTGLWEBCAMDEMO_H
#define QTGLWEBCAMDEMO_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include "face_recog_api/face_recog_api.h"


namespace Ui {
    class QtGLWebcamDemo;
}

class QtGLWebcamDemo : public QMainWindow {
Q_OBJECT

public:
    explicit QtGLWebcamDemo(QWidget* parent = 0);
    ~QtGLWebcamDemo() override;

private:
    std::unique_ptr<Ui::QtGLWebcamDemo> ui;
};

#endif // QTGLWEBCAMDEMO_H
