#ifndef HTTPSYNCDOWNLOADER_H
#define HTTPSYNCDOWNLOADER_H

#include <QObject>
#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>

#include "downloaderresult.h"

namespace Tools
{

// TODO: merge with filedownloader
// TODO: https support
// TODO: https auth support
// TODO: proxy support

class HttpSyncDownloader : public QObject
{
    Q_OBJECT
public:

    explicit HttpSyncDownloader(QObject *parent = nullptr);
    SPDownloaderResult get(QUrl url);
    QString getLastError() const;
signals:

public slots:
    void onHttpFinished();
    void onHttpReadyRead();
    void onAuthenticationRequired(QNetworkReply*,QAuthenticator *);
#ifndef QT_NO_SSL
    void onSSLErrors(QNetworkReply* r,const QList<QSslError> &errors);
#endif

private:
    void logError(const QString& msg);
    static QString getSURL(QNetworkReply* r);
    QNetworkAccessManager m_qnam;
    QNetworkReply* m_reply;
    bool m_httpRequestAborted;
    bool m_isReqFinished;
    SPDownloaderResult m_pLastResult;
};
}
#endif // HTTPSYNCDOWNLOADER_H
