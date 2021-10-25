#include "trafficcounterlimits.h"
#include "tools.h"

using namespace Tools;
/*    uint activeDownloads;
    uint downloaderInstances;
    uint threads;*/
TrafficCounterLimits::TrafficCounterLimits():
    activeDownloads(0),
    downloaderInstances(0),
    threads(0)
{

}

TrafficCounterLimits::TrafficCounterLimits(const TrafficCounterLimits& other):
    activeDownloads(other.activeDownloads),
    downloaderInstances(other.downloaderInstances),
    threads(other.threads)
{

}

TrafficCounterLimits::~TrafficCounterLimits()
{

}

TrafficCounterLimits& TrafficCounterLimits::operator=(const TrafficCounterLimits& other)
{
    if (this != &other)
    {
        activeDownloads = other.activeDownloads;
        downloaderInstances = other.downloaderInstances;
        threads = other.threads;
    }
    return *this;
}

bool TrafficCounterLimits::operator==(const TrafficCounterLimits& other) const
{
    return activeDownloads == other.activeDownloads &&
            downloaderInstances == other.downloaderInstances &&
            threads == other.threads;
}

void TrafficCounterLimits::setToUnlimited()
{
    activeDownloads = 0;
    downloaderInstances = 0;
    threads = 0;
}

QString TrafficCounterLimits::toString() const
{
    return QString("activeDownloads: %1, downloaderInstances: %2, threads: %3").arg(activeDownloads).arg(downloaderInstances).arg(threads);
}

void TrafficCounterLimits::print() const
{
    LOG_TRACE(QString("TrafficCounterLimits[p%1]: %2").arg(intptr_t(this)).arg(toString()));
}
