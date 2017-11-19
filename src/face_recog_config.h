//
// Created by viruch on 14.11.17.
//

#ifndef FACERECOGCLIENTCPP_CONFIG_H
#define FACERECOGCLIENTCPP_CONFIG_H

#include <string>
#include <stdexcept>
#include <QString>
#include <QUrl>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>

struct face_recog_config{
    QUrl embedding_host_url;
    QUrl tracking_host_url;
    QUrl tracking_view_url;
    QString location_name;
    std::string predictor_path;
    static QString path;
};

inline face_recog_config fetch_config(){
    QSettings settings(face_recog_config::path, QSettings::IniFormat);
    auto c = face_recog_config{};
    c.embedding_host_url = QUrl{settings.value("embedding_host_url").toString()};
    c.tracking_host_url = QUrl{settings.value("tracking_host_url").toString()};
    c.tracking_view_url = QUrl{settings.value("tracking_view_url").toString()};
    c.location_name = settings.value("location_name").toString();
    c.predictor_path = settings.value("predictor_path").toString().toStdString();
    return c;
}

#endif //FACERECOGCLIENTCPP_CONFIG_H
