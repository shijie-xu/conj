#include "conjugate.h"
#include <QEventLoop>

Conjugate::Conjugate(QObject *parent) : QObject(parent)
{

}

void Conjugate::setup(QString interface){
    this->interface = interface;
}

QString Conjugate::conj(QString verb){
    QNetworkAccessManager acmgr;
    const QUrl url = interface + verb;
    QNetworkRequest req(url);
    QNetworkReply *rep = acmgr.get(req);

    QEventLoop eventLoop;
    connect(rep, &QNetworkReply::finished,
            &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    QString result = rep->readAll().data();
    rep->deleteLater();
    return result;
}
