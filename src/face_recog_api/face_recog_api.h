#ifndef FACERECOGCLIENTCPP_FACE_RECOG_API_H
#define FACERECOGCLIENTCPP_FACE_RECOG_API_H

#include <QtCore/QObject>
#include <QIODevice>
#include <QNetworkAccessManager>
#include <QUrl>
#include <opencv2/core/core.hpp>
#include <string>
#include "face_recog_config.h"

class face_recog_api
        : public QObject {
Q_OBJECT
public:
    explicit face_recog_api(QObject *parent = nullptr);
    void request_embedding(QByteArray const &  jpg_buffer);
    void track(QByteArray const & jpg_buffer, QByteArray const & embedding, std::vector<cv::Rect> const & positions);

    QNetworkAccessManager networkManager{};
private:
    face_recog_config config;
    QUrl embedding_url;
    QUrl tracking_url;
};

#endif // face_recog_api_H
