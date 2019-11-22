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
    QString get(QString post);
    void unsync_get(QString post);
    void set_trans_url(QString url);
private:
    QString interface;
    QString trans_words;

signals:
    void TranslationComplete(QString trans);
public slots:
    void doWork(){
        unsync_get(trans_words);
    }
};

#endif // CONJUGATE_H
