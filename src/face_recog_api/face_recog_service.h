//
// Created by viruch on 14.11.17.
//

#ifndef FACERECOGCLIENTCPP_FACE_RECOG_SERVICE_H
#define FACERECOGCLIENTCPP_FACE_RECOG_SERVICE_H

#include <QtCore/QObject>
#include <QNetworkAccessManager>
#include <opencv2/core/core.hpp>

#include "face_recog_api.h"

enum class service_state { ready, requesting_embedding, requesting_track };

class face_recog_service
        : public QObject {
Q_OBJECT
public:
    explicit face_recog_service(QObject *parent = nullptr);
    void recognize(cv::Mat const & mat, std::vector<cv::Rect> const & faces);
private:
    void grow_buffer(size_t size);
    void process_reply(QNetworkReply * reply);
    void process_embedding_reply(QNetworkReply * reply);
    void process_track_reply(QNetworkReply * reply);

    cv::Size const face_dimension;
    face_recog_api api;
    std::vector<cv::Mat> faces_buffer;
    std::vector<cv::Rect> faces_positions;
    std::vector<unsigned char> image;
    std::vector<std::vector<unsigned char>> jpg_buffer;
    service_state state;
};

#endif //FACERECOGCLIENTCPP_FACE_RECOG_SERVICE_H
