#include "asyncmetricscollector.h"
#include "toollibTypesInc.h"

using namespace Tools;

CONST_LITERAL CATEGORY_OTHER_FILEPATH("other.txt");

AsyncMetricsCollector::categoryInfo AsyncMetricsCollector::OTHER_CATEGORY_INFO(CATEGORY_OTHER_FILEPATH);
/*
 * AsyncMetricsCollector::Writer Impl
 */

AsyncMetricsCollector::Writer::Writer(AsyncMetricsCollector* owner):
    m_owner(owner)
{
    if (m_owner == nullptr)
    {
        LOG_ERROR("null owner");
    }

}

void AsyncMetricsCollector::Writer::run()
{
    static AsyncMetricsCollector::categoryInfo OTHER_CATEGORY_INFO(CATEGORY_OTHER_FILEPATH);
    LOG_TRACE("writer[ptr%p::tid%p] ENTRY", (void*)this, (void*)QThread::currentThreadId());
    if (m_owner == nullptr)
    {
        LOG_ERROR("writer[ptr%p::tid%p] null owner. exit", (void*)this, (void*)QThread::currentThreadId());
        return;
    }

    QQueue<IMetricRecord*> tmpQueue;
    while(!isInterruptionRequested())
    {
        tmpQueue.clear();
        XMUTEX_LOCK(m_owner->m_recordsLock);
        if (m_owner->m_records.isEmpty())
        {
            XMUTEX_UNLOCK(m_owner->m_recordsLock);
            QThread::msleep(100);
            continue;
        }
        tmpQueue.swap(m_owner->m_records);
        XMUTEX_UNLOCK(m_owner->m_recordsLock);
        //LOG_TRACE("writer[ptr%p::tid%p] %d items are ready to write", (void*)this, (void*)QThread::currentThreadId(), tmpQueue.size());
        while(!tmpQueue.isEmpty())
        {
            IMetricRecord* record = tmpQueue.last();
            tmpQueue.removeLast();
            if (record == nullptr)
            {
                LOG_WARNING("writer[ptr%p::tid%p] null element skip", (void*)this, (void*)QThread::currentThreadId());
                continue;
            }
            categoryInfo& info = (m_owner->m_categories.contains(record->category())?m_owner->m_categories[record->category()]:OTHER_CATEGORY_INFO);
            if (info.path.isEmpty())
            {
                LOG_WARNING("writer[ptr%p::tid%p] empty path for category %u", (void*)this, (void*)QThread::currentThreadId(), record->category());
                continue;
            }
            if (info.file == nullptr)
            {
                info.file = new QFile();
            }

            if (!info.file->isOpen())
            {
                info.file->setFileName(info.path);
                bool openRc = info.file->open(QFile::Append | QIODevice::Text);
                if (!openRc)
                {
                    LOG_ERROR("writer[ptr%p::tid%p] file %s cannot be opened for append. stop writer", (void*)this, (void*)QThread::currentThreadId(), SCHR(info.path));
                    break;
                }
            }
            QString strRow;
            bool first = true;
            // write a header if needed
            if (!info.isUsed)
            {
                QStringList header = record->GetHeader();

                if (!header.isEmpty())
                {
                    for (QString hname: header)
                    {
                        if (!first)
                        {
                            strRow += ",";
                        }
                        first = false;
                        if (hname.contains(','))
                        {
                            strRow += QString("\"%1\"").arg(hname);
                        }
                        else
                        {
                            strRow += hname;
                        }
                    } //for (QString hname: header)
                    info.file->write(strRow.append(ENDL_UNIX).toLatin1());
                } // if (!header.isEmpty())
            } // if (!info.isUsed)

            //write a row
            info.isUsed = true;
            strRow.clear();
            first = true;
            QStringList row = record->GetStringRow();
            if (row.isEmpty())
            {
                continue;
            }
            for (QString rname: row)
            {
                if (!first)
                {
                    strRow += ",";
                }
                first = false;
                if (rname.contains(','))
                {
                    strRow += QString("\"%1\"").arg(rname);
                }
                else
                {
                    strRow += rname;
                }
            } //for (QString hname: header)
            strRow += ENDL_UNIX;
            info.file->write(strRow.toLatin1());
            info.file->flush();
            if (m_owner->m_writeMode == WriteMode::OpenOnWrite)
            {
                info.file->close();
            }
        } // while(tmpQueue.isEmpty())
    } // while(!isInterruptionRequested())
    LOG_TRACE("writer[ptr%p::tid%p] EXIT", (void*)this, (void*)QThread::currentThreadId());
}

/*
 * AsyncMetricsCollector::Writer Impl - END
 */


AsyncMetricsCollector::AsyncMetricsCollector(WriteMode writeMode):
    m_writer(this),
    m_isActive(false),
    m_writeMode(writeMode)
{

}

AsyncMetricsCollector::~AsyncMetricsCollector()
{
    destroy();
}


bool AsyncMetricsCollector::init(const QMap<uint, QString>& categories, const QString& baseDir)
{
    bool bRet = false;
    do
    {
        QMap<uint, categoryInfo> tmpList;
        if (baseDir.trimmed().isEmpty())
        {
            LOG_ERROR("base dir is empty");
            break;
        }
        m_baseDir = CTools::pathNormalize(baseDir.trimmed());

        for (uint k: categories.keys())
        {
            QString v = categories.value(k).trimmed();
            if (!v.isEmpty())
            {
                categoryInfo info(CTools::pathNormalize(v));
                tmpList.insert(k, categoryInfo(CTools::pathNormalize(m_baseDir + "\\" + v)));
            }
        }
        if (tmpList.isEmpty())
        {
            LOG_ERROR("no one valid category in list");
            break;
        }

        tmpList.swap(m_categories);
        bRet = true;
    }
    while(false);
    m_isActive = bRet;
    return bRet;
}

void AsyncMetricsCollector::destroy()
{
    m_isActive = false;
    CTools::SyncThreadTerminate(&m_writer);

    for (categoryInfo info : m_categories.values())
    {
        if (info.file != nullptr)
        {
            info.file->close();
            info.file->deleteLater();
            info.file = nullptr;
        }
    }

    if (OTHER_CATEGORY_INFO.file != nullptr)
    {
        OTHER_CATEGORY_INFO.file->close();
        OTHER_CATEGORY_INFO.file->deleteLater();
        OTHER_CATEGORY_INFO.file = nullptr;
    }
}

void AsyncMetricsCollector::push(IMetricRecord* record)
{
    static quint64 missedRecords = 0;
    do
    {
        if (record == nullptr)
        {
            LOG_WARNING("null record");
            break;
        }

        if (!m_isActive)
        {
            LOG_ERROR("metrics collector is not active. RECORD WILL BE MISSED [%llu]", missedRecords);
            missedRecords++;
            break;
        }

        XMUTEX_LOCK(m_recordsLock);
        m_records.prepend(record);
        XMUTEX_UNLOCK(m_recordsLock);

        if (!m_writer.isRunning())
        {
            m_writer.start();
        }
    }
    while(false);
}
