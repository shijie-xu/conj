#include "conjugate.h"
#include <QEventLoop>
#include <QSslConfiguration>

Conjugate::Conjugate(QObject *parent) : QObject(parent)
{

}

void Conjugate::setup(QString interface){
    this->interface = interface;
}

QString Conjugate::conj(QString verb){
    return get(this->interface+verb);
}

QString Conjugate::get(QString post)
{
    QNetworkAccessManager acmgr;
    const QUrl url = post;
    QNetworkRequest req;

    QSslConfiguration conf = req.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1SslV3);
    req.setSslConfiguration(conf);
    req.setUrl(url);
    QNetworkReply *rep = acmgr.get(req);

    QEventLoop eventLoop;
    connect(rep, &QNetworkReply::finished,
            &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    QString result = rep->readAll().data();
    rep->deleteLater();
    return result;
}

void Conjugate::unsync_get(QString post)
{
    emit TranslationComplete(get(post));
    finishedWork();
}

void Conjugate::set_trans_url(QString url)
{
    this->trans_words = url;
}
