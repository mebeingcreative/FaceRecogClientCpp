#ifndef FACERECOGCLIENTCPP_FACE_RECOG_API_H
#define FACERECOGCLIENTCPP_FACE_RECOG_API_H

#include <QtCore/QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <opencv2/core/core.hpp>
#include <string>

#include "face_recog_config.h"


class face_recog_api
        : public QObject {
Q_OBJECT
public:
    explicit face_recog_api(QObject *parent = nullptr);
    void request_embedding(std::vector<std::vector<unsigned char>> const &  jpg_buffer, int n_of_faces);
    void track(std::vector<unsigned char> const & jpg_buffer, QByteArray const & embedding, std::vector<cv::Rect> const & positions);

    QNetworkAccessManager networkManager{};
private:
    face_recog_config config;
    QUrl embedding_url;
    QUrl tracking_url;
};

#endif // face_recog_api_H
