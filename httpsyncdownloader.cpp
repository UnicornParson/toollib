#include "httpsyncdownloader.h"
#include "tools.h"

using namespace Tools;

HttpSyncDownloader::HttpSyncDownloader(QObject *parent) :
    QObject(parent),
    m_reply(nullptr),
    m_httpRequestAborted(false),
    m_isReqFinished(false),
    m_pLastResult(nullptr)
{
//    connect(&m_qnam, &QNetworkAccessManager::finished,
//            this, &HttpSyncDownloader::onHttpFinished);


    connect(&m_qnam, &QNetworkAccessManager::authenticationRequired,
            this, &HttpSyncDownloader::onAuthenticationRequired);
#ifndef QT_NO_SSL
    connect(&m_qnam, &QNetworkAccessManager::sslErrors,
            this, &HttpSyncDownloader::onSSLErrors);
#endif

    m_pLastResult = std::make_shared<DownloaderResult>(this);
}

void HttpSyncDownloader::logError(const QString& msg)
{
    if (m_pLastResult == nullptr)
    {
        m_pLastResult->appendErrorString(msg);
    }
    m_httpRequestAborted = true;
    LOG_ERROR(msg);
}



SPDownloaderResult HttpSyncDownloader::get(QUrl url)
{
    if (m_pLastResult.get() == nullptr)
    {
        m_pLastResult = std::make_shared<DownloaderResult>(this);
    }

    do
    {
        m_pLastResult->reset();
        m_pLastResult->setRc(eBaseErrorCodes::InProgress);
        m_pLastResult->setRemote(url);
        m_isReqFinished = false;
        m_httpRequestAborted = false;
        if (!url.isValid())
        {
            logError(QString("invalid url %1").arg(url.toString()));
            break;
        }
        QNetworkRequest request(url);
        LOG_TRACE(QString("try to request %1").arg(url.toString()));
        request.setRawHeader("User-Agent", CHR(NETWORK_UDER_AGENT_NAME));
        request.setRawHeader("X-Custom-User-Agent", CHR(NETWORK_UDER_AGENT_NAME));
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        request.setRawHeader("Cache-Control", "no-cache");
        request.setRawHeader("Accept-Encoding", "gzip");

        TickCounter requestDuration;
        m_reply = m_qnam.get(request);
        if (m_reply == nullptr)
        {
            logError("null replay");
        }
        connect(m_reply, &QNetworkReply::finished, this, &HttpSyncDownloader::onHttpFinished);
        connect(m_reply, &QIODevice::readyRead, this, &HttpSyncDownloader::onHttpReadyRead);
        QEventLoop loop;
        connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        m_pLastResult->setRequestDelay(requestDuration.tickToMs());
        m_pLastResult->updateResultSize();
        if (m_httpRequestAborted)
        {
            m_pLastResult->setResultSize(0);
            m_pLastResult->resetRawResult();
            logError("error when fetching");
            break;
        }
        LOG_TRACE("done. size: %zu, duration: %llu", m_pLastResult->resultSize(), m_pLastResult->requestDelay());
        m_pLastResult->setRc(eBaseErrorCodes::NoError);

    }
    while(false);
    return m_pLastResult;
}

QString HttpSyncDownloader::getLastError() const
{
    if (m_pLastResult == nullptr)
    {
        return QString();
    }
    else
    {
        return m_pLastResult->errorString();
    }
}

void HttpSyncDownloader::onHttpFinished()
{
    LOG_TRACE("req done");
    if (m_pLastResult == nullptr)
    {
        logError("no result object");
        return;
    }
    if (m_reply == nullptr)
    {
        logError("no replay object");
        return;
    }
    if (m_reply->error())
    {
        logError(m_reply->errorString());
    }
    else
    {
        LOG_TRACE("request finished");
#ifdef NETWORK_TRACE //for network debug
        LOG_TRACE(QString("SURL: %1").arg(getSURL(m_reply)));
        LOG_TRACE(QString("ContentTypeHeader: %1").arg(m_reply->header(QNetworkRequest::ContentTypeHeader).toString()));
        LOG_TRACE(QString("LastModifiedHeader: %1").arg(m_reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString()));
        LOG_TRACE(QString("ContentLengthHeader: %1").arg(m_reply->header(QNetworkRequest::ContentLengthHeader).toULongLong()));
        LOG_TRACE(QString("HttpStatusCodeAttribute: %1").arg(m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()));
        LOG_TRACE(QString("HttpReasonPhraseAttribute: %1").arg(m_reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()));
#endif
    }
    QByteArray dataPart = m_reply->readAll();
    LOG_TRACE("received %d", dataPart.size());
    if (!dataPart.isEmpty())
    {
        m_pLastResult->appendRawResult(dataPart);
        m_pLastResult->updateResultSize();
    }
    m_reply->deleteLater();
    m_reply = nullptr;
}

void HttpSyncDownloader::onHttpReadyRead()
{
    QByteArray dataPart = m_reply->readAll();
    if (m_pLastResult == nullptr)
    {
        logError("no result object");
        return;
    }
    LOG_TRACE("received %d", dataPart.size());
    if (!dataPart.isEmpty())
    {
        m_pLastResult->appendRawResult(dataPart);
        m_pLastResult->updateResultSize();
    }
}

void HttpSyncDownloader::onAuthenticationRequired(QNetworkReply*,QAuthenticator *)
{
    logError("AuthenticationRequired but not supported");
}

QString HttpSyncDownloader::getSURL(QNetworkReply* r)
{
    if (r != nullptr)
    {
        return r->url().toString();
    }
    return QString();
}

#ifndef QT_NO_SSL
void HttpSyncDownloader::onSSLErrors(QNetworkReply* r, const QList<QSslError> &errors)
{
    m_httpRequestAborted = true;

    QString msg = QString("SSL error on url %1: ").arg(getSURL(r));
    foreach (QSslError e, errors)
    {
        msg += e.errorString() + "; ";
    }
    logError(msg);
}

#endif

