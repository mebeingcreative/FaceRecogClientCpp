#include "qviewergl.h"
#include <QDebug>


QViewerGl::QViewerGl(QWidget* parent) :
        QOpenGLWidget(parent)
{}

void QViewerGl::initializeGL() {
    float const vertices[] = {
            // Position      Texture
            -1.0f, 1.0f, 1.0f, 0.0f, // Top-left
            1.0f, 1.0f, 0.0f, 0.0f, // Top-right
            1.0f, -1.0f, 0.0f, 1.0f, // Bottom-right

            1.0f, -1.0f, 0.0f, 1.0f, // Bottom-right
            -1.0f, -1.0f, 1.0f, 1.0f, // Bottom-left
            -1.0f, 1.0f, 1.0f, 0.0f  // Top-left
    };

    char const* vertexSource =
            R"glsl(
            #version 130

            in vec2 position;
            in vec2 texcoord;

            out vec2 Texcoord;

            uniform vec2 trans;

            void main(){
                Texcoord = texcoord;
                gl_Position = vec4(trans * position, 0.0, 1.0);
            }

            )glsl";

    char const* fragmentSource =
            R"glsl(
            #version 130

            in vec2 Texcoord;

            out vec4 outColor;

            uniform sampler2D tex;

            void main(){
                outColor = texture(tex, Texcoord);
            }

            )glsl";

    initializeOpenGLFunctions();

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    initializeProgram(vertexSource, fragmentSource);

    auto const posAttrib = static_cast<GLuint>(glGetAttribLocation(shaderProgram, "position"));
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float), 0);

    auto const texAttrib = static_cast<GLuint>(glGetAttribLocation(shaderProgram, "texcoord"));
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float), (void*) (2 * sizeof(float)));

    auto const transUniform = glGetUniformLocation(shaderProgram, "trans");
    //glUniform2f(transUniform, 1.0f, 1.0f);
    setWidgetAspectRatio(width(), height());

    glGenTextures(1, &tex);

    float const bordercolor[] = {.2f, .2f, .2f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    qDebug() << glGetError();
}

void QViewerGl::resizeGL(int width, int height) {
    setWidgetAspectRatio(width, height);
    emit imageSizeChanged(width, height);
    updateScene();
}

void QViewerGl::updateScene() {
    if (isVisible()) {
        update();
    }
}

void QViewerGl::paintGL() {
    renderImage();
}

void QViewerGl::renderImage() {
    drawMutex.lock();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    drawMutex.unlock();
}

void QViewerGl::initializeProgram(char const * vertexSource, char const * fragmentSource){
    auto const vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    auto const fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    shaderProgram = static_cast<GLuint>(glCreateProgram());
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
}

GLuint QViewerGl::compileShader(char const* source, GLenum const type) {
    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, nullptr, buffer);
        qDebug() << buffer;
    }
    return shader;
}


void QViewerGl::setCamAspectRatio(int width, int height){
    camAspectRatio = (float) width / (float) height;
    recalculateAspect();
}

void QViewerGl::setWidgetAspectRatio(int width, int height){
    widgetAspectRatio = (float) width / (float) height;
    recalculateAspect();
}

void QViewerGl::recalculateAspect() {
    auto const transUniform = glGetUniformLocation(shaderProgram, "trans");
    if (widgetAspectRatio < camAspectRatio){
        glUniform2f(transUniform,
                    1.0f,
                    widgetAspectRatio / camAspectRatio);
    } else {
        glUniform2f(transUniform,
                    camAspectRatio / widgetAspectRatio,
                    1.0f);
    }
}

bool QViewerGl::showImage(cv::Mat const & image) {
    drawMutex.lock();
    if (image.channels() == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR,
                     GL_UNSIGNED_BYTE, image.data);
        glBindTexture(GL_TEXTURE_2D, tex);
        setCamAspectRatio(image.cols, image.rows);
    } else {
        qDebug() << "Camera mode not supported: " << image.channels();
        drawMutex.unlock();
        return false;
    }

    updateScene();
    drawMutex.unlock();
    return true;
}
