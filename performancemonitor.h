#ifndef PERFORMANCEMONITOR_H
#define PERFORMANCEMONITOR_H

#include "performancesensor.h"

#include <QList>
#include <QHash>
#include <QDateTime>

#include "xmutex.h"

namespace Tools
{

typedef QList<MeasurementTime_t> MeasurementSet_t;
typedef QHash<QString, MeasurementSet_t > RecordsContainer_t;

struct metricsStat
{
    MeasurementTime_t min = 0;
    MeasurementTime_t max = 0;
    MeasurementTime_t avg = 0;
    quint64 count = 0;
};

struct metricsPack
{
    QHash<QString, metricsStat> metrics;
    MeasurementTime_t uptime;
    int measurementCount;
};

class PerformanceMonitor
{
public:
    static void addMeasurement(const PerformanceMeasurement& newData);
    static metricsPack buildMetrics();
    static void printMetrics(const metricsPack& pack);
    static bool saveAsSCV(const QString& path, const metricsPack& pack);
    static void reset();
    virtual ~PerformanceMonitor();
private:
    static metricsStat getStat(const MeasurementSet_t& set);
    PerformanceMonitor();
    RecordsContainer_t m_data;
    QList<PerformanceMeasurement> data;
    XRecursiveMutex m_dataMutex;
    QDateTime startTime;
    static PerformanceMonitor instance;
}; // class PerformanceMonitor
} // namespace Tools
#endif // PERFORMANCEMONITOR_H
