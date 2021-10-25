#include "trafficcounter.h"

TrafficCounter::TrafficCounter(QObject *parent) : QObject(parent)
{

}

TrafficCounter& TrafficCounter::self()
{
    static TrafficCounter counter;
    return counter;
}
