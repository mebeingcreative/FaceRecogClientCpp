//
// Created by viruch on 14.11.17.
//

#ifndef FACERECOGCLIENTCPP_CONFIG_H
#define FACERECOGCLIENTCPP_CONFIG_H

#include <string>
#include <QString>
#include <QUrl>
#include <QSettings>

struct face_recog_config{
    QString location_name;
    QUrl embedding_service_url;
    QUrl tracking_api_url;
    QUrl tracking_view_url;
    std::string predictor_path;
    static QString path;
};

inline face_recog_config fetch_config(){
    QSettings settings(face_recog_config::path, QSettings::IniFormat);
    auto c = face_recog_config{};
    c.location_name = settings.value("location_name").toString();
    c.embedding_service_url = QUrl{settings.value("embedding_service_url").toString()};
    c.tracking_api_url = QUrl{settings.value("tracking_api_url").toString()};
    c.tracking_view_url = QUrl{settings.value("tracking_view_url").toString().replace("$location_name", c.location_name)};
    c.predictor_path = settings.value("predictor_path").toString().toStdString();
    return c;
}

#endif //FACERECOGCLIENTCPP_CONFIG_H
