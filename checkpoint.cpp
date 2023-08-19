#include "checkpoint.h"
#include "CuteLogger/Logger.h"
#include "badwaytrap.h"
#include "json.h"
#include "qglobal.h"
#include "qthread.h"
#include "xmutex.h"
#include "xmutexguard.h"
#include <QCryptographicHash>
#include <tuple>

using namespace Tools;

CONST_LITERAL FrameEntryMessage("FRAME-ENTRY");
CONST_LITERAL FrameExitMessage("FRAME-EXIT");

CheckpointsFrame::CheckpointsFrame(const QString& tag) noexcept :
    QObject(nullptr), m_tag(tag)
{

}
    CheckpointsFrame::~CheckpointsFrame()
    {
        CheckpointsCollector::leaveFrame(m_tag);
    }

QString CheckpointsFrame::makeLongTag(const QString& here)
{
    static qint64 n = 0;
    static XNonRecursiveMutex m;

    QCryptographicHash h(QCryptographicHash::Algorithm::Md5);
    h.addData(here.toUtf8());
    h.addData(THREAD_STR_ID.toUtf8());
    XMutexGuard g(&m);
    qint64 id = n++;
    const QString optHere = here.trimmed().isEmpty() ? "nowhere" : here.trimmed();
    return QString("<%1>%2.%3 - %4").arg(id).arg(h.result().toHex(), THREAD_STR_ID, optHere);
}

void CheckpointsFrame::makeCheckpoint(const QString& name)
{
    emit checkpoint(name);
    emit CheckpointsCollector::instance().checkpoint(name, m_tag);
}

CheckpointsCollector::CheckpointsCollector()
    : QObject{nullptr}
{
    connect(this, &CheckpointsCollector::checkpoint, &CheckpointsCollector::onCheckpoint);
}

void CheckpointsCollector::enterFrame(const QString& frame)
{
    if (frame.isEmpty())
    {
        LOG_ERROR("empty frame name");
        return;
    }
    emit CheckpointsCollector::instance().checkpoint(frame, FrameEntryMessage);
}
void CheckpointsCollector::leaveFrame(const QString& frame)
{
    if (frame.isEmpty())
    {
        LOG_ERROR("empty frame name");
        return;
    }
    emit CheckpointsCollector::instance().checkpoint(frame, FrameExitMessage);
}

void CheckpointsCollector::onCheckpoint(const QString& name, const QString& frame)
{
    XMutexGuard g(&m_logMutex);
    m_log.push_back(std::make_tuple(QDateTime::currentMSecsSinceEpoch(), frame, name));
}

CheckpointsCollector& CheckpointsCollector::instance()
{
    static CheckpointsCollector self;
    return self;
}

QtJson::JsonObject CheckpointsCollector::serialize()
{
    QtJson::JsonObject obj;
    QtJson::JsonArray arr;
    XMutexGuard g(&m_logMutex);
    for (const checkpointRecord_t& record : m_log)
    {
        const QtJson::JsonObject rObj =
        {
            {"when", std::get<0>(record)},
            {"where", std::get<1>(record)},
            {"what", std::get<2>(record)}
        };
        arr.append(rObj);
    }
    obj.insert("records", arr);
    return obj;
}

bool CheckpointsCollector::deserialize(const QtJson::JsonObject& in)
{
    Q_UNUSED(in)
    BADWAY_TRAP("NOT DESERIALIZABLE OBJECT");
    return false;
}

void CheckpointsCollector::reset()
{
    XMutexGuard g(&m_logMutex);
    m_log.clear();
}

void CheckpointsCollector::play(const std::function<void(const CheckpointsCollector::checkpointRecord_t&)>& func)
{
    XMutexGuard g(&m_logMutex);
    for (const checkpointRecord_t& record : m_log)
    {
        func(record);
    }
}
