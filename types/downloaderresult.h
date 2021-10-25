#ifndef DOWNLOADERRESULT_H
#define DOWNLOADERRESULT_H

#include <memory>

#include <QObject>
#include <QUrl>

#include "downloaderresultcode.h"

namespace Tools
{

// TODO: add unit tests
// TODO: add printer and serializator for dumping to file

class DownloaderResult: public QObject
{
    Q_OBJECT

public:
    enum TargetType
    {
        TargetTypeBuffer = 0,
        TargetTypeFile
    };

    Q_ENUM(TargetType)
    DownloaderResult(QObject* parent = nullptr);
    DownloaderResult(const DownloaderResult& other);
    DownloaderResult& operator=(const DownloaderResult& other);
    ~DownloaderResult();
    void reset();

    QString printableString() const;
    eBaseErrorCodes rc() const;
    int serverCode() const;
    size_t resultSize() const;
    DownloaderResult::TargetType targetType() const;
    QByteArray rawResult() const;
    QString src() const;
    QString dst() const;
    quint64 requestDelay() const;
    QString errorString() const;
    QUrl remote() const;

    void setRc(eBaseErrorCodes rc);
    void setServerCode(int code);
    void setResultSize(size_t size);
    void updateResultSize();
    void setTargetType(DownloaderResult::TargetType type);
    void setRawResult(const QByteArray& rawData);
    void appendRawResult(const QByteArray& rawData);
    void resetRawResult();
    void setSrc(const QString& src);
    void setDst(const QString& dst);
    void setRequestDelay(quint64 delay);
    void setErrorString(const QString& e);
    void appendErrorString(const QString& e);
    void resetErrorString();
    void setRemote(const QUrl& url);

signals:
    void rcChanged(eBaseErrorCodes rc);
    void serverCodeChanged(int code);
    void resultSizeChanged(size_t size);
    void targetTypeChanged(TargetType type);
    void rawResultChanged(const QByteArray& rawData);
    void srcChanged(const QString& src);
    void dstChanged(const QString& dst);
    void requestDelayChanged(quint64 delay);
    void errorStringChanged(const QString& e);
    void remoteChanged(const QUrl& url);

private:
    static QString targetTypeToString(DownloaderResult::TargetType type);
    void emitAll();

    eBaseErrorCodes m_rc;
    int m_serverCode;
    size_t m_resultSize;
    TargetType m_targetType;
    QByteArray m_rawResult;
    QString m_src;
    QString m_dst;
    quint64 m_requestDelay;
    QString m_errorString;
    QUrl m_remote;
};

typedef std::shared_ptr<DownloaderResult> SPDownloaderResult;
}
#endif // DOWNLOADERRESULT_H
