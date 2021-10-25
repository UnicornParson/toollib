#include "performancesensor.h"
#include "performancemonitor.h"
#include "CuteLogger/cuteloggerinc.h"

using namespace Tools;

QString PerformanceMeasurement::getToken() const
{
    return QString("%1::%2.%3(%4)").arg(className, functionName).arg(line).arg(ext);
}

PerformanceSensor::PerformanceSensor(const QString &className, const QString &functionName, int line, const QString &ext)
{
    data.className = className;
    data.functionName = functionName;
    data.line = line;
    data.ext = ext;
    ticker.reset();
}

PerformanceSensor::~PerformanceSensor()
{

}

void PerformanceSensor::destroy()
{
    //report metric to monitor
    data.time = ticker.tickToMs();
    PerformanceMonitor::addMeasurement(data);
}

