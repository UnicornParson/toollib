#ifndef TRAFFICCOUNTERLIMITS_H
#define TRAFFICCOUNTERLIMITS_H

#include <QtGlobal>
#include <QString>

namespace Tools
{
class TrafficCounterLimits
{
public:
    uint activeDownloads;
    uint downloaderInstances;
    uint threads;

    TrafficCounterLimits();
    TrafficCounterLimits(const TrafficCounterLimits& other);
    ~TrafficCounterLimits();
    TrafficCounterLimits& operator=(const TrafficCounterLimits& other);
    bool operator==(const TrafficCounterLimits& other) const;
    void setToUnlimited();
    QString toString() const;
    void print() const;
}; // class TrafficCounterLimits
} // namespace Tools
#endif // TRAFFICCOUNTERLIMITS_H
