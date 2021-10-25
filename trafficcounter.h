#ifndef TRAFFICCOUNTER_H
#define TRAFFICCOUNTER_H

#include <QObject>
#include "toollibTypesInc.h"
namespace Tools
{
class TrafficCounter : public QObject
{
    Q_OBJECT
public:


signals:

public slots:

private:
    explicit TrafficCounter(QObject *parent = nullptr);
    static TrafficCounter& self();

    TrafficCounterLimits m_limits;
    QAtomicInt
}; // class TrafficCounter
} // namespace Tools
#endif // TRAFFICCOUNTER_H
