//
// Created by viruch on 14.11.17.
//

#ifndef FACERECOGCLIENTCPP_CONFIG_H
#define FACERECOGCLIENTCPP_CONFIG_H

#include <QSettings>

struct config{
    explicit config(
            QString const & embeddingHost,
            QString const & trackingHost,
            QString const & location
    ):
            embeddingHost{embeddingHost},
            trackingHost{trackingHost},
            location{location}
    {}
    QString const embeddingHost;
    QString const trackingHost;
    QString const location;
};

template<typename T>
config fetch_config(T&& filename){
    QSettings settings(std::forward<T>(filename), QSettings::IniFormat);
    return config(
            settings.value("embedding_host").toString(),
            settings.value("tracking_host").toString(),
            settings.value("location").toString()
    );
}

#endif //FACERECOGCLIENTCPP_CONFIG_H
