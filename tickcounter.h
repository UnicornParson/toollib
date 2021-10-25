#ifndef TICKCOUNTER_H
#define TICKCOUNTER_H
#include <QString>
#include <QDateTime>
namespace Tools
{
class TickCounter
{
public:
  TickCounter();
  QString tickToString();
  quint64 tickToMs();
  void reset();

  static QString msToString(quint64 ms);
private:
  QDateTime lastTime;
};
}
#endif // TICKCOUNTER_H
