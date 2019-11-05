#ifndef CONJUGATE_H
#define CONJUGATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class Conjugate : public QObject
{
    Q_OBJECT
public:
    explicit Conjugate(QObject *parent = nullptr);

signals:

public slots:

public:
    void setup(QString interface);
    QString conj(QString verb);
private:
    QString interface;
};

#endif // CONJUGATE_H
