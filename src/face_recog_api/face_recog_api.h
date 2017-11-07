#ifndef face_recog_api_H
#define face_recog_api_H

#include <QtCore/QObject>
#include <QIODevice>
#include <QNetworkAccessManager>
#include <functional>
#include <string>

class face_recog_api : public QObject {
Q_OBJECT
public:
    explicit face_recog_api(std::string const & host, QObject *parent = nullptr);
    void request_embedding(QByteArray const & jpg_buffer);
    void track(QByteArray const & jpg_buffer, QByteArray embedding);
private:
    QString host;
    QNetworkAccessManager networkManager{};
};

#endif // face_recog_api_H
