#include <QApplication>
#include <QException>
#include <QDebug>
#include "face_recog_config.h"
#include "face_recog_demo.h"

QString face_recog_config::path = "./face-recog-config.ini";

int main(int argc, char* argv[]) {
    int res = -1;
    if (argc > 1){
        face_recog_config::path = QString{argv[1]};
    }
    try {
        QApplication a(argc, argv);
        face_recog_demo w{};
        w.showMaximized();

        res = a.exec();
    } catch (QException & e) {
        qCritical() << QString("Exception: %1").arg(e.what());
    } catch (std::exception & e) {
        qCritical() << QString("Exception: %1").arg(e.what());
    }

    return res;
}


