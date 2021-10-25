#ifndef MSTIMER_H
#define MSTIMER_H

#include <QObject>
#include <QTimer>
#include <QList>
#include "tools.h"
namespace Tools
{
  MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR int DEFAULT_TIMER_INTERVAL(100);

  class IMSTimerObserver
  {
  public:
    virtual void onMsTimer(int id, msTime_t time) = 0;
    virtual ~IMSTimerObserver();
  };

  class MSTimer : public QObject
  {
    Q_OBJECT
  public:
    explicit MSTimer(int interval = DEFAULT_TIMER_INTERVAL , QObject *parent = nullptr);
    void addObserver(IMSTimerObserver *ptr);
    void removeObserver(IMSTimerObserver *ptr);
    void removeAllObservers();
    inline bool hasObserver(IMSTimerObserver *ptr) const { return m_observers.contains(ptr);}
    inline bool isActive() const { return m_timer.isActive();}
    inline int timerId() const { return m_timer.timerId();}
    inline void setInterval(int msec) { m_timer.setInterval(msec);}
    inline int interval() const { return m_timer.interval();}
    inline int remainingTime() const;

    void start() {m_timer.start();  LOG_TRACE("on start");}
    void stop() {m_timer.stop();  LOG_TRACE("on stop");}
  private slots:
    void onTimeout();
  private:
    QTimer m_timer;
    QList<IMSTimerObserver*> m_observers;
  };
}
#endif // MSTIMER_H
