//
// Created by viruch on 14.11.17.
//

#include <QNetworkReply>
#include <opencv2/opencv.hpp>

#include "face_recog_service.h"

std::vector<unsigned char> & convert_to_jpeg(cv::Mat const & mat, std::vector<unsigned char> & out) {
    std::vector<int> params{ cv::IMWRITE_JPEG_QUALITY, 95 };
    out.clear();
    cv::imencode(".jpg", mat, out, params);
    return out;
}

face_recog_service::face_recog_service(QObject *parent):
    QObject(parent),
    api{},
    faces_buffer{},
    face_image_size{160, 160},
    state{service_state::ready}
{
    grow_buffer(4);
    connect(&(api.networkManager), &QNetworkAccessManager::finished,
            this, &face_recog_service::process_reply);
}

void face_recog_service::recognize(cv::Mat const & mat, std::vector<cv::Rect> const & faces) {
    if (state != service_state::ready) {
        return;
    }
    faces_positions = faces;
    auto n_of_faces = faces_positions.size();
    grow_buffer(n_of_faces);
    for (int i{0}; i < n_of_faces; ++i){
        resize(mat(faces_positions[i]), faces_buffer[i], face_image_size);
        convert_to_jpeg(faces_buffer[i], jpg_buffer[i]);
    }
    api.request_embedding(jpg_buffer, n_of_faces);

    convert_to_jpeg(mat, image);
    state = service_state::requesting_embedding;
}

void face_recog_service::grow_buffer(size_t const size){
    while (faces_buffer.size() < size){
        faces_buffer.emplace_back(face_image_size, CV_32SC3);
        jpg_buffer.emplace_back(8 * 1000);
    }
}

void face_recog_service::process_reply(QNetworkReply * const reply){
    auto error = reply->error();

    if (error != QNetworkReply::NoError){
        qCritical() << error;
        state = service_state::ready;
        return;
    }

    if (state == service_state::requesting_embedding){
        process_embedding_reply(reply);
    } else if (state == service_state::requesting_track){
        process_track_reply(reply);
    } else {
        qCritical() << "face_recog_service is in an invalid state";
        state = service_state::ready;
    }
}

void face_recog_service::process_embedding_reply(QNetworkReply * const reply){
    auto const embedding = reply->readAll();
    api.track(image, embedding, faces_positions);
    state = service_state::requesting_track;
}

void face_recog_service::process_track_reply(QNetworkReply * const reply){
    state = service_state::ready;
}