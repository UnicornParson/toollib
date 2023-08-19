#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <QObject>
#include "interfaces.h"
#include "qlist.h"
#include "tools.h"
#include "xmutex.h"

namespace Tools
{

class CheckpointsFrame : public QObject
{
    Q_OBJECT
public:
    explicit CheckpointsFrame(const QString& tag) noexcept;
    ~CheckpointsFrame();
    QString tag() const {return m_tag;}

    static QString makeLongTag(const QString& here = "");
    void makeCheckpoint(const QString& name);
signals:
    void checkpoint(const QString& name);
private:
    QString m_tag;
}; // CheckpointsFrame

#define InitCheckpointsFrame() \
    Tools::CheckpointsFrame __c_frame(Tools::CheckpointsFrame::makeLongTag(__HERE__)); \
    CheckpointsCollector::enterFrame(__c_frame.tag())

#define ReachCheckpoint(name) __c_frame.makeCheckpoint(name)

class CheckpointsCollector : public QObject, public IJSerializable
{
    Q_OBJECT
public:
    typedef quint64 timpeshtamp_t;

    /// time - frame - checkpoint
    typedef std::tuple<timpeshtamp_t, QString, QString> checkpointRecord_t;

    CheckpointsCollector();

    static void enterFrame(const QString& frame);
    static void leaveFrame(const QString& frame);
    static CheckpointsCollector& instance();

    virtual QtJson::JsonObject serialize() Q_DECL_OVERRIDE;
    virtual bool deserialize(const QtJson::JsonObject& in) Q_DECL_OVERRIDE;

    void reset();

    /// exec func for each log record
    void play(const std::function<void (const CheckpointsCollector::checkpointRecord_t &)> &func);

signals:
    /// emits for each new checkpoint
    void checkpoint(const QString& name, const QString& frame);

private slots:
    void onCheckpoint(const QString& name, const QString& frame);

private:
    XNonRecursiveMutex m_logMutex;
    QList<checkpointRecord_t> m_log;
};
}; // namespace Tools
#endif // CHECKPOINT_H
