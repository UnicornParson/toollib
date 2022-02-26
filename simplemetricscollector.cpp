#include "simplemetricscollector.h"
#include <QDir>

using namespace Tools;

SimpleMetricsCollector SimpleMetricsCollector::m_instance;

SimpleMetricsCollector::SimpleMetricsCollector():
    m_baseDirPath(QString()),
    m_metricsPath(QString()),
    m_isEnabled(false)
{

}

SimpleMetricsCollector::~SimpleMetricsCollector()
{
    destroy();
}
void SimpleMetricsCollector::destroy()
{
    m_isEnabled = false;
    if (m_file.isOpen())
    {
        m_file.close();
    }
}

bool SimpleMetricsCollector::init(const QString& baseDirPath)
{
    bool bRet = false;
    do
    {
        m_instance.m_isEnabled = false;
        m_instance.m_baseDirPath = CTools::pathNormalize(baseDirPath.trimmed());
        if (m_instance.m_baseDirPath.isEmpty())
        {
            m_instance.m_baseDirPath = ".";
        }
        if (m_instance.m_baseDirPath.right(1) != CTools::getPathSeparator())
        {
            m_instance.m_baseDirPath += CTools::getPathSeparator();
        }
        QDir d;
        if (!d.exists(baseDirPath))
        {
            bool mkdirRc = d.mkdir(baseDirPath);
            if (!mkdirRc)
            {
                LOG_ERROR(QString("Cannot create metrics directory in %1").arg(baseDirPath));
                break;
            }
        }
        QString fileName = QString("%1metrics_%2.txt").arg(m_instance.m_baseDirPath, QDateTime::currentDateTime().toString("dd.MM.yyyy_hh.mm.ss"));
        int index = 0;
        while(QFile::exists(fileName))
        {
            fileName = QString("%1metrics_%2.%3.txt").arg(m_instance.m_baseDirPath).arg(index).arg(QDateTime::currentDateTime().toString("dd.MM.yyyy_hh.mm.ss"));
            index++;
        }
        m_instance.m_metricsPath = fileName;
        m_instance.m_file.setFileName(fileName);
        if (!m_instance.m_file.open(QFile::Append))
        {
            LOG_ERROR(QString("cannot open file %1 for append").arg(fileName));
            break;
        }
        LOG_TRACE(QString("save metrics to %1").arg(fileName));
        m_instance.m_isEnabled = true;
        bRet = true;
    }
    while(false);
    return bRet;
}

void SimpleMetricsCollector::add(const QString& name, QVariantMap params)
{
    if (m_instance.m_isEnabled)
    {
        QString sValue;
        for (const QVariant& param: params)
        {
            sValue += param.toString().append(";");
        }

        m_instance.post(name, QString("%1").arg(sValue));
    }
}

void SimpleMetricsCollector::add(const QString& name, qint64 value)
{
    if (m_instance.m_isEnabled)
    {
        m_instance.post(name, QString("%1").arg(value));
    }
}

void SimpleMetricsCollector::add(const QString& name, int value)
{
    if (m_instance.m_isEnabled)
    {
        m_instance.post(name, QString("%1").arg(value));
    }
}
void SimpleMetricsCollector::add(const QString& name, quint64 value)
{
    if (m_instance.m_isEnabled)
    {
        m_instance.post(name, QString("%1").arg(value));
    }
}

void SimpleMetricsCollector::add(const QString& name, const QString& value)
{
    if (m_instance.m_isEnabled)
    {
        m_instance.post(name, value);
    }
}

void SimpleMetricsCollector::post(const QString& name, const QString& value)
{
    if (!m_instance.m_isEnabled)
    {
        return;
    }
    QString msg = QString("%1;%2;%3\n")
            .arg(QDateTime::currentMSecsSinceEpoch())
            .arg(name, value);

    XMUTEX_LOCK(m_fileMutex);
    m_file.write(msg.toLatin1());
    m_file.flush();
    XMUTEX_UNLOCK(m_fileMutex);
}

