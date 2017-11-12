#ifndef face_recog_api_H
#define face_recog_api_H

#include <QtCore/QObject>
#include <QRect>
#include <QIODevice>
#include <QNetworkAccessManager>
#include <string>

class face_recog_api
        : public QObject {
Q_OBJECT
public:
    explicit face_recog_api(std::string const & host, QObject *parent = nullptr);
    void request_embedding(QByteArray const & jpg_buffer, QRect position);
    void track(QByteArray const & jpg_buffer, QByteArray const & embedding, QRect position);
private:
    QString host;
    QNetworkAccessManager networkManager{};
};

#endif // face_recog_api_H
