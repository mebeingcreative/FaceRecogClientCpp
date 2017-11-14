//
// Created by viruch on 14.11.17.
//

#ifndef FACERECOGCLIENTCPP_CONFIG_H
#define FACERECOGCLIENTCPP_CONFIG_H

#include <QString>
#include <QUrl>
#include <QSettings>

struct face_recog_config{
    face_recog_config(
            QUrl const & embedding_host_url,
            QUrl const & tracking_host_url,
            QUrl const & tracking_view_url,
            QString const & location_name
    ):
            embedding_host_url{embedding_host_url},
            tracking_host_url{tracking_host_url},
            tracking_view_url{tracking_view_url},
            location_name{location_name}
    {}
    QUrl const embedding_host_url;
    QUrl const tracking_host_url;
    QUrl const tracking_view_url;
    QString const location_name;
};

inline face_recog_config fetch_config(){
    QSettings settings("config.ini", QSettings::IniFormat);
    return {
            QUrl{settings.value("embedding_host_url").toString()},
            QUrl{settings.value("tracking_host_url").toString()},
            QUrl{settings.value("tracking_view_url").toString()},
            settings.value("location_name").toString()
    };
}

#endif //FACERECOGCLIENTCPP_CONFIG_H
