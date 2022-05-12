#include "tickcounter.h"

using namespace Tools;

TickCounter::TickCounter() noexcept
{
  reset();
}


TickCounter::TickCounter(const TickCounter& other):
    m_lastTime(other.m_lastTime)
{

}

TickCounter::TickCounter(const TickCounter&& other)
{
    m_lastTime = std::move(other.m_lastTime);
}

TickCounter& TickCounter::operator=(const TickCounter& other)
{
    if (this != &other)
    {
        m_lastTime = other.m_lastTime;
    }
    return *this;
}

void TickCounter::reset()
{
  m_lastTime = QDateTime::currentDateTimeUtc();
}

QString TickCounter::tickToString()
{
  return msToString(tickToMs());

}

QString TickCounter::msToString(quint64 ms)
{
  quint64 _ms = ms % 1000;
  quint64 _s = (ms / 1000) % 60;
  quint64 _m = (ms / (1000 * 60)) % 60;
  quint64 _h = (ms / (1000 * 60 * 60));
  return QString().asprintf("%02llu:%02llu:%02llu.%03llu", _h, _m, _s, _ms);
}

quint64 TickCounter::tickToMs()
{
  qint64 delta = m_lastTime.msecsTo(QDateTime::currentDateTimeUtc());
  reset();
  return (quint64)delta;
}
