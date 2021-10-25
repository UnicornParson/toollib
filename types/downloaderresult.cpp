#include "downloaderresult.h"
#include "memcontrol.h"
#include "tools.h"

using namespace Tools;

QString DownloaderResult::targetTypeToString(DownloaderResult::TargetType type)
{
    return Tools::QEnumToString<DownloaderResult::TargetType>(type);
}

DownloaderResult::DownloaderResult(QObject *parent):
    QObject(parent),
    m_rc(eBaseErrorCodes::Unknown),
    m_serverCode(200),
    m_resultSize(0),
    m_targetType(TargetType::TargetTypeBuffer),
    m_requestDelay(0)
{
    ADD_POINTER_SENSOR(this);
}

DownloaderResult::~DownloaderResult()
{
    REMOVE_POINTER_SENSOR(this);
}

DownloaderResult::DownloaderResult(const DownloaderResult& other):
    QObject(other.parent()),
    m_rc(other.m_rc),
    m_serverCode(other.m_serverCode),
    m_resultSize(other.m_resultSize),
    m_targetType(other.m_targetType),
    m_rawResult(other.m_rawResult),
    m_src(other.m_src),
    m_dst(other.m_dst),
    m_requestDelay(other.m_requestDelay)
{
    ADD_POINTER_SENSOR(this);
}

DownloaderResult& DownloaderResult::operator=(const DownloaderResult& other)
{
    if (this != &other)
    {
        m_rc = other.m_rc;
        m_serverCode = other.m_serverCode;
        m_resultSize = other.m_resultSize;
        m_targetType = other.m_targetType;
        m_rawResult = other.m_rawResult;
        m_src = other.m_src;
        m_dst = other.m_dst;
        m_requestDelay = other.m_requestDelay;
    }
    return *this;
}

void DownloaderResult::reset()
{
    m_rc = eBaseErrorCodes::Unknown;
    m_serverCode = 200;
    m_resultSize = 0;
    m_targetType = TargetType::TargetTypeBuffer;
    m_requestDelay = 0;
    m_rawResult.clear();
    m_src.clear();
    m_dst.clear();
    m_errorString.clear();
    m_remote.clear();
    emitAll();
}

void DownloaderResult::emitAll()
{
    emit rcChanged(rc());
    emit serverCodeChanged(serverCode());
    emit resultSizeChanged(resultSize());
    emit targetTypeChanged(targetType());
    emit rawResultChanged(rawResult());
    emit srcChanged(src());
    emit dstChanged(dst());
    emit requestDelayChanged(requestDelay());
    emit errorStringChanged(errorString());
    emit remoteChanged(remote());
}

QString DownloaderResult::printableString() const
{
    QString msg = QString("DownloaderResult{ rc: %1, targetType: %2, serverCode: %3, src: %4, dst: %5, delay: %6, error: %7, remote: %8 }")
            .arg(BaseErrorCodes::toString(m_rc))
            .arg(targetTypeToString(m_targetType))
            .arg(m_serverCode)
            .arg(m_src).arg(m_dst)
            .arg(m_requestDelay)
            .arg(m_errorString)
            .arg(m_remote.toString());

    LOG_DATA_OBJECT_GETTER(DownloaderResult, msg);
    return msg;
}

eBaseErrorCodes DownloaderResult::rc() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, BaseErrorCodes::toString(m_rc));
    return m_rc;
}

int DownloaderResult::serverCode() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, m_serverCode);
    return m_serverCode;
}

size_t DownloaderResult::resultSize() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, (uint)m_resultSize);
    return m_resultSize;
}

DownloaderResult::TargetType DownloaderResult::targetType() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, targetTypeToString(m_targetType));
    return m_targetType;
}

QByteArray DownloaderResult::rawResult() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, m_rawResult.size());
    return m_rawResult;
}

QString DownloaderResult::src() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, m_src);
    return m_src;
}

QString DownloaderResult::dst() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, m_dst);
    return m_dst;
}

quint64 DownloaderResult::requestDelay() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, m_requestDelay);
    return m_requestDelay;
}

QString DownloaderResult::errorString() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, m_errorString);
    return m_errorString;
}

QUrl DownloaderResult::remote() const
{
    LOG_DATA_OBJECT_GETTER(DownloaderResult, m_remote.toString());
    return m_remote;
}

void DownloaderResult::setRc(eBaseErrorCodes rc)
{
    if (m_rc != rc)
    {
        m_rc = rc;
        LOG_TRACE("rc is changed. new value %s", SCHR(BaseErrorCodes::toString(rc)));
        emit rcChanged(m_rc);
    }
}

void DownloaderResult::setServerCode(int code)
{
    if (m_serverCode != code)
    {
        m_serverCode = code;
        LOG_TRACE("server code is changed. new value %d", m_serverCode);
        emit serverCodeChanged(m_serverCode);
    }
}

void DownloaderResult::setResultSize(size_t size)
{
    if (m_resultSize != size)
    {
        m_resultSize = size;
        LOG_TRACE("result size is changed. new value %zu", m_resultSize);
        emit resultSizeChanged(m_resultSize);
    }
}

void DownloaderResult::updateResultSize()
{
    setResultSize(m_rawResult.size());
}

void DownloaderResult::setTargetType(DownloaderResult::TargetType type)
{
    if (m_targetType != type)
    {
        m_targetType = type;
        LOG_TRACE("target type is changed. new value %s", SCHR(targetTypeToString(m_targetType)));
        emit targetTypeChanged(m_targetType);
    }
}

void DownloaderResult::setRawResult(const QByteArray& rawData)
{
    if (m_rawResult.compare(rawData) != 0)
    {
        m_rawResult = rawData;
        LOG_TRACE("raw data is changed. new size %lld", rawData.size());
        emit rawResultChanged(m_rawResult);
    }
}

void DownloaderResult::appendRawResult(const QByteArray& rawData)
{
    if (!rawData.isEmpty())
    {
        m_rawResult.append(rawData);
        LOG_TRACE("raw data is changed. new size %lld", m_rawResult.size());
        emit rawResultChanged(m_rawResult);
    }
}

void DownloaderResult::resetRawResult()
{
    if (!m_rawResult.isEmpty())
    {
        m_rawResult.clear();
        LOG_TRACE("raw data is changed. new size 0 (RESET DATA)");
        emit rawResultChanged(m_rawResult);
    }
}

void DownloaderResult::setSrc(const QString& src)
{
    if (m_src.compare(src) != 0)
    {
        m_src = src;
        LOG_TRACE("src is changed. new value %s", SCHR(m_src));
        emit srcChanged(m_src);
    }
}

void DownloaderResult::setDst(const QString& dst)
{
    if (m_dst.compare(dst) != 0)
    {
        m_dst = dst;
        LOG_TRACE("dst is changed. new value %s", SCHR(m_dst));
        emit dstChanged(m_dst);
    }
}

void DownloaderResult::setRequestDelay(quint64 delay)
{
    if (m_requestDelay != delay)
    {
        m_requestDelay = delay;
        LOG_TRACE("request delay is changed. new value %llu", m_requestDelay);
        emit requestDelayChanged(m_requestDelay);
    }
}

void DownloaderResult::setErrorString(const QString& e)
{
    if (m_errorString.compare(e) != 0)
    {
        m_errorString = e;
        LOG_TRACE("error string is changed. new value %s", SCHR(m_errorString));
        emit errorStringChanged(m_errorString);
    }
}


void DownloaderResult::appendErrorString(const QString& e)
{
    if (e.isEmpty())
    {
        return;
    }
    setErrorString(errorString() + QString("; ") + e);
}

void DownloaderResult::resetErrorString()
{
    LOG_TRACE("reset error string");
    if (!m_errorString.isEmpty())
    {
        m_errorString.clear();
        emit errorStringChanged(QString());
    }
}

void DownloaderResult::setRemote(const QUrl &url)
{
    if (m_remote != url)
    {
        m_remote = url;
        LOG_TRACE("remote url is changed. new value %s", SCHR(url.toString()));
        emit remoteChanged(m_remote);
    }
}

