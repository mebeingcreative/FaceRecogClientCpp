#include "face_recog_api.h"

#include <iostream>
#include <QtNetwork>
#include <QDebug>

face_recog_api::face_recog_api(std::string const & host, QObject *parent):
        host{QString::fromStdString(host)},
        QObject(parent){

//    connect(&networkManager, &QNetworkAccessManager::finished,
//            [](auto * const response){
//                std::cout << "start" << QString(response->readAll()).toStdString() << "end";
//            });
}

void face_recog_api::request_embedding(QByteArray const & image_buffer) {
    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart{};
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant(R"(form-data; name="images[]"; filename="001.jpg")"));

    imagePart.setBody(image_buffer);

    multiPart->append(imagePart);

    auto const qurl = QUrl{host + "/embed"};
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

#include "face_recog_api.moc"
