#ifndef PERFORMANCESENSOR_H
#define PERFORMANCESENSOR_H

#include <QString>

#include "tickcounter.h"

namespace Tools
{
typedef quint64 MeasurementTime_t;
struct PerformanceMeasurement
{
    QString className;
    QString functionName;
    int line;
    QString ext;
    MeasurementTime_t time;

    QString getToken() const;
};

class PerformanceSensor
{
public:
    PerformanceSensor(const QString& className, const QString& functionName, int line, const QString& ext);
    void destroy();
    virtual ~PerformanceSensor();
private:
    TickCounter ticker;
    PerformanceMeasurement data;
}; //class PerformanceSensor

#ifdef PERF_MON
#define ADD_PERF_SENSOR Tools::PerformanceSensor _perfSensor(__FILE__, Q_FUNC_INFO, __LINE__, "")
#define ADD_PERF_SENSOR_EX(ext) Tools::PerformanceSensor _perfSensor(__FILE__, Q_FUNC_INFO, __LINE__, ext)
#define DESTROY_PERF_SENSOR _perfSensor.destroy()
#else
#define ADD_PERF_SENSOR
#define ADD_PERF_SENSOR_EX(ext)
#define DESTROY_PERF_SENSOR
#endif

} //namespace Tools
#endif // PERFORMANCESENSOR_H
