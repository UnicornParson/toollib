#include "performancemonitor.h"

#include <limits>

#include "CuteLogger/cuteloggerinc.h"

using namespace Tools;

CONST_LITERAL CSV_HEADER("Index;Token;Min;Max;Avg;Count\n");

PerformanceMonitor PerformanceMonitor::instance;

PerformanceMonitor::PerformanceMonitor():
    startTime(QDateTime::currentDateTimeUtc())
{
}

PerformanceMonitor::~PerformanceMonitor()
{
}

void PerformanceMonitor::addMeasurement(const PerformanceMeasurement& newData)
{
#ifndef PERF_MON
    return;
#endif
    XMUTEX_LOCK(instance.m_dataMutex);
    try
    {
        QString token = newData.getToken();
        if (instance.m_data.contains(token))
        {
            instance.m_data[token].append(newData.time);
        }
        else
        {
            MeasurementSet_t set;
            set.append(newData.time);
            instance.m_data.insert(token, set);
        }
    }
    catch(...)
    {
        LOG_ERROR("cannot append data to monitor");
    }
    XMUTEX_UNLOCK(instance.m_dataMutex);
}

void PerformanceMonitor::reset()
{
    XMUTEX_LOCK(instance.m_dataMutex);
    instance.data.clear();
    XMUTEX_UNLOCK(instance.m_dataMutex);
}

void PerformanceMonitor::printMetrics(const metricsPack &pack)
{
#ifndef PERF_MON
    return;
#endif
    LOG_TRACE_LINE;
    LOG_TRACE(QString("PERFORMANCE METRICS"));
    LOG_TRACE(QString("Uptime: %1").arg(TickCounter::msToString(pack.uptime)));
    LOG_TRACE(QString("Full measurement count: %1").arg(pack.measurementCount));
    LOG_TRACE_LINE;
    int traceRowIndex = 0;
    foreach(QString token, pack.metrics.keys())
    {
        metricsStat stat = pack.metrics.value(token);
        LOG_TRACE(QString(" - [%1] %2 - min: %3, max: %4, avg: %5, count: %6")
                  .arg(traceRowIndex)
                  .arg(token)
                  .arg(stat.min)
                  .arg(stat.max)
                  .arg(stat.avg)
                  .arg(stat.count));
        traceRowIndex++;
    }

    LOG_TRACE_LINE;
}


metricsStat PerformanceMonitor::getStat(const MeasurementSet_t& set)
{
    double avg = 0.0;
    metricsStat stat;
    const double dsize = double(set.size());
    stat.max = 0;
    stat.min = 0;
    stat.count = quint64( set.size());
    foreach(MeasurementTime_t time, set)
    {
        avg += double(time) / dsize;
        stat.max = qMax<MeasurementTime_t>(stat.max, time);
        stat.min = qMin<MeasurementTime_t>(stat.min, time);
    }
    stat.avg = MeasurementTime_t(avg);
    return stat;
}

metricsPack PerformanceMonitor::buildMetrics()
{
    metricsPack pack;
#ifndef PERF_MON
    return pack;
#endif
    RecordsContainer_t dataClone;

    pack.uptime = instance.startTime.msecsTo(QDateTime::currentDateTimeUtc());

    XMUTEX_LOCK(instance.m_dataMutex);
    dataClone = instance.m_data;
    XMUTEX_UNLOCK(instance.m_dataMutex);

    int dataSize = dataClone.size();
    pack.measurementCount = dataSize;
    LOG_TRACE(QString("Measurements count: %1").arg(dataSize));
    metricsStat stat;
    foreach (QString token, dataClone.keys())
    {
        stat = getStat(dataClone.value(token));
        pack.metrics.insert(token, stat);
    }
    return pack;
}

bool PerformanceMonitor::saveAsSCV(const QString& path, const metricsPack& pack)
{
#ifndef PERF_MON
    return false;
#endif
    bool ret = false;
    do
    {
        if (path.isEmpty())
        {
            LOG_ERROR("empty path");
            break;
        }
        QFile f(path);
        ret = f.open(QIODevice::WriteOnly);
        if (!ret)
        {
            LOG_ERROR("cannot open file %s for write", SCHR(path));
            break;
        }
        //Index,Token,Min,Max,Avg
        f.write(QString(CSV_HEADER).toLatin1());
        quint64 traceRowIndex = 0;
        foreach(QString token, pack.metrics.keys())
        {
            metricsStat stat = pack.metrics.value(token);
            f.write(QString("%1;%2;%3;%4;%5;%6\n")
                      .arg(traceRowIndex)
                      .arg(token)
                      .arg(stat.min)
                      .arg(stat.max)
                      .arg(stat.avg)
                      .arg(stat.count).toLatin1());
            traceRowIndex++;
        }
        f.flush();
        f.close();
        ret = true;
    }
    while(false);
    return ret;
}
