#ifndef QTGLWEBCAMDEMO_H
#define QTGLWEBCAMDEMO_H

#include <QMainWindow>
#include <memory>
#include <opencv2/highgui/highgui.hpp>

namespace Ui {
    class QtGLWebcamDemo;
}

class QtGLWebcamDemo : public QMainWindow {
Q_OBJECT

public:
    explicit QtGLWebcamDemo(QWidget* parent = 0);
    ~QtGLWebcamDemo();

protected:
    void timerEvent(QTimerEvent* event);

private slots:
    void on_actionStart_triggered();

private:
    std::unique_ptr<Ui::QtGLWebcamDemo> ui;
    cv::VideoCapture capture;
};

#endif // QTGLWEBCAMDEMO_H
