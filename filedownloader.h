#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H
#include <QNetworkReply>
#include <QObject>
#include <QSslError>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QFile>
#include "objectid.h"
#include "toollibTypesInc.h"
#include "xmutex.h"

namespace Tools
{
  class FileDownloader : public QObject
  {
    Q_OBJECT
  public:
    explicit FileDownloader(IDownloaderObserver* pObserver = nullptr);
    bool SyncDownload(const QString& urlStr,
                      const QString& target,
                      DownloaderResult* rc = nullptr,
                      bool bOverwrite = false,
                      const QString& localPath = ".");

    void SetObserver(IDownloaderObserver* pObserver) { m_pObserver = pObserver;}
    inline IDownloaderObserver* observer() const {return m_pObserver;}
    inline bool IsActive() const {return m_queryInProgress;}
    DownloaderResult* result() const {return m_pLastResult;}
    inline QString lastError() const {return m_sLastError;}
    void allowLocalDownloads(bool val) {m_allowLocalDownloads = val;}
    bool localDownloadsAllowed() const {return m_allowLocalDownloads;}
    void setDefaultLocalStoragePath(QString& path);
    QString defaultLocalStoragePath() const {return m_localStoragePath;}
  public slots:
    void OnCancelDownload();
    void OnHttpFinished();
    void OnHttpReadyRead();
    void OnReplyFinished(QNetworkReply* pReply);
#ifndef QT_NO_SSL
    void OnSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
#endif
  private:
    void logError(const QString& msg);
    void rcDestroy();
    DownloaderResult* m_pLastResult;
    IDownloaderObserver* m_pObserver;
    QString m_sLastError;
    QWaitCondition m_replayCondition;
    XRecursiveMutex m_downloadMutex;
    QFile* m_pTargetFile;
    bool m_alarmState;
    bool m_queryInProgress;
    bool m_allowLocalDownloads;
    QNetworkAccessManager m_qnam;
    QNetworkReply* m_reply;
    QString m_localStoragePath;
  };


  class DLJob: public QThread
  {
  public:
    struct Query
    {
      QString urlStr;
      QString target;
      DownloaderResult rc;
      bool bOverwrite;
      bool bNoLocal;
      QString localStorage;

      Query():Query("", ""){;}
      Query(const QString& _urlStr, const QString& _target, bool _bOverwrite = false, bool _bNoLocal = false):
          urlStr(_urlStr),
          target(_target),
          bOverwrite(_bOverwrite),
          bNoLocal(_bNoLocal){;}

      inline bool isValid() const {return !(urlStr.isEmpty() || target.isEmpty());}
    };

    explicit DLJob(IDownloaderObserver* pObserver = nullptr);
    ~DLJob() Q_DECL_OVERRIDE;
    inline bool isActive() const {return m_engine.IsActive() && isRunning(); }
    inline DownloaderResult* result() const {return m_engine.result();}
    bool exec(const Query& query);
    inline QString id() const {return m_id.str();}
    void cancel();
    inline QString remoteUrl() const {return m_query.urlStr;}
    inline QString targetPath() const {return m_query.target;}
  private:
    void run() Q_DECL_OVERRIDE;
    char* jobMarker();
    Query m_query;
    FileDownloader m_engine;
    IDownloaderObserver* m_pObserver;
    ObjectId m_id;
  };
} //namespcae tools
#endif // FILEDOWNLOADER_H
