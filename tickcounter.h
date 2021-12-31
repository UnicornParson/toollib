#ifndef TICKCOUNTER_H
#define TICKCOUNTER_H
#include <QString>
#include <QDateTime>

namespace Tools
{
class TickCounter
{
public:
  TickCounter() noexcept;
  TickCounter(const TickCounter& other);
  TickCounter(const TickCounter&& other);
  TickCounter& operator=(const TickCounter& other);

  QString tickToString();
  quint64 tickToMs();
  void reset();

  static QString msToString(quint64 ms);
private:
  QDateTime m_lastTime;
}; // class TickCounter
} // namespace Tools
#endif // TICKCOUNTER_H
