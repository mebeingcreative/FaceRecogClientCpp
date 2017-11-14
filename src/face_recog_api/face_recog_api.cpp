#include "face_recog_api.h"

#include <iostream>
#include <QtNetwork>
#include <QDebug>

#include "face_recog_api.moc"

face_recog_api::face_recog_api(std::string const & host, QObject *parent):
        host{QString::fromStdString(host)},
        QObject(parent){
}

void face_recog_api::request_embedding(QByteArray const &  jpg_buffer) {
    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart{};
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant(R"(form-data; name="images[]"; filename="001.jpg")"));

    imagePart.setBody(jpg_buffer);

    multiPart->append(imagePart);

    auto const qurl = QUrl{host + "/embed"};
    auto const request = QNetworkRequest{qurl};

    QNetworkReply * reply = networkManager.post(request, multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply
}

void face_recog_api::track(QByteArray const & jpg_buffer, QByteArray const & embedding, std::vector<cv::Rect> const & positions) {
    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    auto jsonDoc = QJsonDocument::fromJson(embedding);
    auto jsonObj = QJsonObject{
            {"location", "cam2"},
            {"positions", QJsonArray{
                    QJsonObject{
                            {"top", positions[0].y},
                            {"left", positions[0].x},
                            {"width", positions[0].width},
                            {"height", positions[0].height}
                    }
            }},
            {"embeddings", jsonDoc.object()["embedding"].toArray()}
    };
    jsonDoc.setObject(jsonObj);

    QHttpPart jsonPart{};
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(R"(form-data; name="data")"));
    jsonPart.setBody(jsonDoc.toJson(QJsonDocument::JsonFormat::Compact));

    QHttpPart imagePart{};
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant(R"(form-data; name="image"; filename="001.jpg")"));

    imagePart.setBody(jpg_buffer);

    multiPart->append(jsonPart);
    multiPart->append(imagePart);

    auto const qurl = QUrl{"https://face.otep.ch/track"};
    auto const request = QNetworkRequest{qurl};

    QNetworkReply * reply = networkManager.post(request, multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply

    connect(reply, &QNetworkReply::finished,
            [reply](){
                qDebug() << QString(reply->readAll());
            });

    connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            [](auto const code){
                qCritical() << "request failed: " << code;
            });
}
