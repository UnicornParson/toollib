#ifndef METRICSCOLLECTOR_H
#define METRICSCOLLECTOR_H

#include <QThread>
#include <QStringList>
#include <QMap>
#include <QQueue>
#include <QString>
#include <QFile>
#include "tools.h"
#include "xmutex.h"
#include "toollibTypesInc.h"

namespace Tools
{
class AsyncMetricsCollector
{
public:
    enum class WriteMode: int
    {
        OpenOnWrite = 0,
        KeepOpened,
        Auto = OpenOnWrite
    };
    AsyncMetricsCollector(WriteMode writeMode = WriteMode::Auto);
    ~AsyncMetricsCollector();

    bool init(const QMap<uint, QString>& categories, const QString& baseDir);
    inline bool isActive() const {return m_isActive;}
    void destroy();
    void push(IMetricRecord *record);
private:

    class Writer: public QThread
    {
    public:
        explicit Writer(AsyncMetricsCollector* owner);
        void run();
    private:
        AsyncMetricsCollector* m_owner;
    };
    struct categoryInfo
    {
        explicit categoryInfo(QString _path):
            path(_path),
            file(nullptr),
            isUsed(false) {}

        categoryInfo():
            path(QString("%1.txt").arg((quint64)this)),
            file(nullptr),
            isUsed(false) {}

        QString path;
        QFile* file;
        bool isUsed;
    };
    static categoryInfo OTHER_CATEGORY_INFO;
    QMap<uint, categoryInfo> m_categories;
    QString m_baseDir;
    QQueue<IMetricRecord*> m_records;
    XRecursiveMutex m_recordsLock;
    Writer m_writer;
    bool m_isActive;
    WriteMode m_writeMode;
}; // class AsyncMetricsCollector
}; // namespace Tools
#endif // METRICSCOLLECTOR_H
