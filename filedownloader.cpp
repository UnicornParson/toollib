#include "filedownloader.h"
#include "tools.h"
#include <QFile>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include "tickcounter.h"
#include "simplemetricscollector.h"
#include "buildConstants.h"

using namespace Tools;


FileDownloader::FileDownloader(IDownloaderObserver* pObserver) :
    QObject(nullptr),
    m_pObserver(pObserver),
    m_sLastError(""),
    m_pTargetFile(nullptr),
    m_alarmState(false),
    m_queryInProgress(false),
    m_allowLocalDownloads(false),
    m_reply(nullptr),
    m_localStoragePath(".")
{
    connect(&m_qnam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(OnReplyFinished(QNetworkReply*)));

#ifndef QT_NO_SSL
    connect(&m_qnam, &QNetworkAccessManager::sslErrors,
            this, &FileDownloader::OnSslErrors);
#endif

}
void FileDownloader::logError(const QString& msg)
{
    LOG_ERROR(QString("FileDownloader[%1] error: %2").arg(reinterpret_cast<quint64>(this)).arg(msg));
    m_sLastError = msg;
    m_alarmState = true;
}

void FileDownloader::rcDestroy()
{
    if (m_pTargetFile != nullptr)
    {
        m_pTargetFile->close();
        m_pTargetFile->deleteLater();
        m_pTargetFile = nullptr;
    }
}

void FileDownloader::setDefaultLocalStoragePath(QString& path)
{
    if (m_localStoragePath != path)
    {
        m_localStoragePath = path.trimmed();
        LOG_TRACE("default local storage path is updated. %s", SCHR(m_localStoragePath));
        if (m_localStoragePath.isEmpty())
        {
            LOG_WARNING("new path is empty. set .");
            m_localStoragePath = ".";
        }
    }
}

bool FileDownloader::SyncDownload(const QString& urlStr,
                                  const QString& target,
                                  DownloaderResult* rc,
                                  bool bOverwrite,
                                  const QString &localPath)
{
    TickCounter syncDelay;
    m_downloadMutex.lock();
    SimpleMetricsCollector::add(MetricsAPIKey::FD_METRIC_API_SYNC_WAIT, syncDelay.tickToMs());
    TickCounter reqDelay;
    m_queryInProgress = true;
    DownloaderResult localRc;
    if (rc == nullptr)
    {
        rc = &localRc;
    }
    m_pLastResult = rc;

    rcDestroy();

    rc->setRc(eBaseErrorCodes::Unknown);
    rc->setDst(target);
    rc->setSrc(urlStr);
    do
    {
        const QUrl url = QUrl::fromUserInput(urlStr);
        if (!url.isValid())
        {
            logError(QString("invalid url %1").arg(urlStr));
            rc->setRc(eBaseErrorCodes::InvalidAddress);
            break;
        }
        if (url.isLocalFile())
        {
            if (!m_allowLocalDownloads)
        {
                logError(QString("local downloads are not allowed but %1 is a local file").arg(url.toString()));
                rc->setRc(eBaseErrorCodes::InvalidAddress);
                break;
            }
            if (localPath.isEmpty())
            {
                logError("local path is not set");
                rc->setRc(eBaseErrorCodes::InvalidAddress);
                break;
            }
            QString localSrc = CTools::pathNormalize(localPath.trimmed() + urlStr.trimmed());
            rc->setSrc(localSrc);
            QFile f(localSrc);
            if (!f.exists())
            {
                logError(QString("%1 is not found").arg(localSrc));
                rc->setRc(eBaseErrorCodes::TargetNotFound);
                break;
            }
            TickCounter copyDelay;
            bool copyRc = f.copy(target);
            if (!copyRc)
            {
                logError(QString("%1 cannot by copied to %2").arg(localSrc).arg(target));
                rc->setRc(eBaseErrorCodes::IOError);
                break;
            }
            rc->setRc(eBaseErrorCodes::NoError);
            rc->setResultSize(f.size());
            SimpleMetricsCollector::add(MetricsAPIKey::FD_METRIC_API_FETCH_DELAY, copyDelay.tickToMs());
            break;
        }

        if (!bOverwrite && QFile::exists(target))
        {
            logError(QString("file %1 alredy exists bOverwrite is falce").arg(target));
            rc->setRc(eBaseErrorCodes::FileExists);
            break;
        }
        m_pTargetFile = new QFile(target);
        // create empty file and try to open
        bool openRc = m_pTargetFile->open(QIODevice::ReadWrite);
        if (!openRc)
        {
            logError(QString("file %1 cannot be opened for write").arg(target));
            rc->setRc(eBaseErrorCodes::FSError);
            break;
        }

        QNetworkRequest req(url);
        LOG_TRACE(QString("try to request %1").arg(url.toString()));
        req.setRawHeader("User-Agent", CHR(NETWORK_UDER_AGENT_NAME));
        req.setRawHeader("X-Custom-User-Agent", CHR(NETWORK_UDER_AGENT_NAME));
        req.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        req.setRawHeader("Cache-Control", "no-cache");
        req.setRawHeader("Accept-Encoding", "gzip");
        m_alarmState = false;

        m_reply = m_qnam.get(req);
        if (m_reply == nullptr)
        {
            logError("get error");
            break;
        }

        QEventLoop loop;
        connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (m_alarmState)
        {
            logError("error when fetching");
            break;
        }
        rc->setRc(eBaseErrorCodes::NoError);
    }
    while(false);
    m_queryInProgress = false;
    rcDestroy();
    m_downloadMutex.unlock();
    quint64 operationDelay = reqDelay.tickToMs();
    SimpleMetricsCollector::add(MetricsAPIKey::FD_METRIC_API_FETCH_DELAY, operationDelay);
    rc->setRequestDelay(operationDelay);
    return (rc->rc() == eBaseErrorCodes::NoError);
}

void FileDownloader::OnCancelDownload()
{
    m_reply->abort();
    logError("OnCancelDownload");
}

void FileDownloader::OnHttpFinished()
{
    LOG_TRACE("req done");
    if (!m_pTargetFile->isOpen())
    {
        logError("file is closed");
        return;
    }

    if (m_reply->error())
    {
        logError(m_reply->errorString());
        m_alarmState = true;
        //m_responceBuf.clear();
    }
    else
    {
        LOG_TRACE("request finished");
#ifdef NETWORK_TRACE //for network debug
        LOG_TRACE(QString("ContentTypeHeader: %1").arg(m_reply->header(QNetworkRequest::ContentTypeHeader).toString()));
        LOG_TRACE(QString("LastModifiedHeader: %1").arg(m_reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString()));
        LOG_TRACE(QString("ContentLengthHeader: %1").arg(m_reply->header(QNetworkRequest::ContentLengthHeader).toULongLong()));
        LOG_TRACE(QString("HttpStatusCodeAttribute: %1").arg(m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()));
        LOG_TRACE(QString("HttpReasonPhraseAttribute: %1").arg(m_reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()));
#endif
    }
    QByteArray dataPart = m_reply->readAll();
    LOG_TRACE("received %d", dataPart.size());
    m_pTargetFile->write(dataPart);
    m_pTargetFile->flush();
    m_reply->deleteLater();
    m_reply = nullptr;
}

void FileDownloader::OnHttpReadyRead()
{
    QByteArray dataPart = m_reply->readAll();
    LOG_TRACE("received %d", dataPart.size());
    if (!dataPart.isEmpty())
    {
        m_pTargetFile->write(dataPart);
        m_pTargetFile->flush();
    }
}

void FileDownloader::OnReplyFinished(QNetworkReply* pReply)
{
QString surl;
if(pReply != nullptr)
{
     surl = pReply->url().toString();
}
    LOG_TRACE("replyFinished. target: %s", SCHR(surl));
    OnHttpFinished();
}

#ifndef QT_NO_SSL
void FileDownloader::OnSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    QString surl;
    if (reply != nullptr)
    {
         surl = reply->url().toString();
    }
    QString msg = QString("SSL errors for url %1: \n").arg(surl);
    foreach(QSslError err, errors)
    {
        msg += QString(" - ").append(err.errorString()).append("\n");
    }
    logError(msg);
}
#endif

//==================================================================================================================
// DLJob impl
//==================================================================================================================

DLJob::DLJob(IDownloaderObserver* pObserver):
    QThread(nullptr),
    m_engine(nullptr),//events and results will be handled in this class
    m_pObserver(pObserver)
{
    LOG_TRACE("[%s]: new download job", jobMarker());
}

bool DLJob::exec(const Query& query)
{
    if (!query.isValid())
    {
        LOG_ERROR("[%s]: invalid query", jobMarker());
        return false;
    }
    m_query = query;
    QThread::start();
    return true;
}

char* DLJob::jobMarker()
{
    return SCHR(QString("DLjob%1.%2").arg((quint64)this).arg(id()));
}

void DLJob::cancel()
{
    if (isActive())
    {
        LOG_TRACE("[%s]: cancel job", jobMarker());
        m_engine.OnCancelDownload();
        terminate();
    }
}

DLJob::~DLJob()
{
    cancel();
}

void DLJob::run()
{
    bool lastALD = m_engine.localDownloadsAllowed();
    m_engine.allowLocalDownloads(!m_query.bNoLocal);
    bool dlRc = m_engine.SyncDownload(m_query.urlStr,
                                      m_query.target,
                                      &m_query.rc,
                                      m_query.bOverwrite,
                                      m_query.localStorage);
    m_engine.allowLocalDownloads(lastALD);
    if (!dlRc)
    {
        m_pObserver->OnError(id(), m_engine.lastError(), m_query.rc.rc());
    }
    else
    {
        m_pObserver->OnFinish(id(), m_query.rc);
    }
    LOG_ERROR("[%s]: exit", jobMarker());
}
