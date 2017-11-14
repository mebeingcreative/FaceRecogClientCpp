#include "face_recog_api.h"

#include <QtNetwork>
#include <QDebug>

face_recog_api::face_recog_api(QObject * parent):
        QObject(parent),
        config{fetch_config()}
{
    embedding_url = config.embedding_host_url;
    embedding_url.setPath("/embed");
    tracking_url = config.tracking_host_url;
    tracking_url.setPath("/track");
}

void face_recog_api::request_embedding(QByteArray const &  jpg_buffer) {
    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart{};
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant(R"(form-data; name="images[]"; filename="001.jpg")"));

    imagePart.setBody(jpg_buffer);

    multiPart->append(imagePart);

    auto const request = QNetworkRequest{embedding_url};

    QNetworkReply * reply = networkManager.post(request, multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply
}

void face_recog_api::track(QByteArray const & jpg_buffer, QByteArray const & embedding, std::vector<cv::Rect> const & positions) {
    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    auto jsonPositions = QJsonArray{};

    for(auto const & p : positions){
       jsonPositions.push_back(QJsonObject{
               {"top", p.y},
               {"left", p.x},
               {"width", p.width},
               {"height", p.height}
       });
    }

    auto jsonDoc = QJsonDocument::fromJson(embedding);
    auto jsonObj = QJsonObject{
            {"location", config.location_name},
            {"positions", jsonPositions},
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

    auto const request = QNetworkRequest{tracking_url};

    QNetworkReply * reply = networkManager.post(request, multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply
}
