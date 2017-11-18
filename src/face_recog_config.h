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
    face_recog_config(
            QUrl const & embedding_host_url,
            QUrl const & tracking_host_url,
            QUrl const & tracking_view_url,
            QString const & location_name,
            std::string const & predictor_path
    ):
            embedding_host_url{embedding_host_url},
            tracking_host_url{tracking_host_url},
            tracking_view_url{tracking_view_url},
            location_name{location_name},
            predictor_path{predictor_path}
    {}
    QUrl const embedding_host_url;
    QUrl const tracking_host_url;
    QUrl const tracking_view_url;
    QString const location_name;
    std::string const predictor_path;
};

inline face_recog_config fetch_config(){
    QSettings settings("config.ini", QSettings::IniFormat);
    return {
            QUrl{settings.value("embedding_host_url").toString()},
            QUrl{settings.value("tracking_host_url").toString()},
            QUrl{settings.value("tracking_view_url").toString()},
            settings.value("location_name").toString(),
            settings.value("predictor_path").toString().toStdString()
    };
}

#endif //FACERECOGCLIENTCPP_CONFIG_H
