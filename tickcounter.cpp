#include "tickcounter.h"
using namespace Tools;
TickCounter::TickCounter()
{
  reset();
}

void TickCounter::reset()
{
  lastTime = QDateTime::currentDateTimeUtc();
}

QString TickCounter::tickToString()
{
  return msToString(tickToMs());

}

QString TickCounter::msToString(quint64 ms)
{
  int _ms = ms % 1000;
  int _s = (ms / 1000) % 60;
  int _m = (ms / (1000 * 60)) % 60;
  int _h = (ms / (1000 * 60 * 60));
  return QString().asprintf("%02d:%02d:%02d.%03d", _h, _m, _s, _ms);
}

quint64 TickCounter::tickToMs()
{
  qint64 delta = lastTime.msecsTo(QDateTime::currentDateTimeUtc());
  reset();
  return delta;
}
