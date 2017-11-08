#ifndef QVIEWERGL_H
#define QVIEWERGL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <mutex>

class QViewerGl :
        public QOpenGLWidget {
Q_OBJECT
public:
    explicit QViewerGl(QWidget* parent = 0);
    ~QViewerGl();

signals:
    void imageSizeChanged(int outW, int outH); /// Used to resize the image outside the widget

public slots:
    bool showImage(cv::Mat const & image); /// Used to set the image to be viewed

protected:
    void initializeGL() override;

    void paintGL() override;

    void resizeGL(int width, int height) override;

private:
    void recalculateAspect();

    void initializeProgram(char const * vertexShader, char const * fragmentShader);

    GLuint compileShader(char const* source, GLenum type);

    void renderImage();

    void updateScene();

    void setCamAspectRatio(int width, int height);

    void setWidgetAspectRatio(int width, int height);

    GLuint tex;
    GLuint shaderProgram;
    float camAspectRatio{1.0f};
    float widgetAspectRatio{1.0f};
    std::mutex drawMutex;
};

#endif // CQTOPENCVVIEWERGL_H
