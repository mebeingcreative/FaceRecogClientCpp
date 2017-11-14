//
// Created by viruch on 14.11.17.
//

#include <fstream>
#include <QNetworkReply>
#include "face_recog_service.h"
#include "face_recog_service.moc"
#include <opencv2/opencv.hpp>

std::vector<unsigned char> & convert_to_jpeg(cv::Mat const & mat, std::vector<unsigned char> & out) {
    std::vector<int> params{ cv::IMWRITE_JPEG_QUALITY, 80 };
    out.clear();
    cv::imencode(".jpg", mat, out, params);
    return out;
}

QByteArray wrap_with_qbytearray(std::vector<unsigned char> const & vector){
    return {reinterpret_cast<char const *>(vector.data()), static_cast<int const>(vector.size())};
}

face_recog_service::face_recog_service():
    api{"http://localhost:5001"},
    faces_buffer{},
    face_dimension{182, 182},
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
    grow_buffer(faces_positions.size());
    for (int i{0}; i < faces_positions.size(); ++i){
        resize(mat(faces_positions[i]), faces_buffer[i], face_dimension);
    }
    api.request_embedding(wrap_with_qbytearray(
            convert_to_jpeg(faces_buffer[0], jpg_buffer)));

    convert_to_jpeg(mat, image);
    state = service_state::requesting_embedding;
}

void face_recog_service::grow_buffer(size_t const size){
    while (faces_buffer.size() < size){
        faces_buffer.emplace_back(face_dimension, CV_32SC3);
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
    auto embedding = reply->readAll();
    qDebug() << embedding;
    api.track(wrap_with_qbytearray(image), embedding, faces_positions);
    state = service_state::requesting_track;
}

void face_recog_service::process_track_reply(QNetworkReply * const reply){
    qDebug() << reply->readAll();
    state = service_state::ready;
}