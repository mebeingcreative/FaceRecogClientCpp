#ifndef FACERECOGCLIENTCPP_FACE_RECOG_API_H
#define FACERECOGCLIENTCPP_FACE_RECOG_API_H

#include <QtCore/QObject>
#include <QIODevice>
#include <QNetworkAccessManager>
#include <opencv2/core/core.hpp>
#include <string>

class face_recog_api
        : public QObject {
Q_OBJECT
public:
    explicit face_recog_api(std::string const & host, QObject *parent = nullptr);
    void request_embedding(QByteArray const &  jpg_buffer);
    void track(QByteArray const & jpg_buffer, QByteArray const & embedding, std::vector<cv::Rect> const & positions);

    QNetworkAccessManager networkManager{};
private:
    QString host;
};

#endif // face_recog_api_H
