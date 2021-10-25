#include "mstimer.h"
using namespace Tools;
IMSTimerObserver::~IMSTimerObserver(){}

MSTimer::MSTimer(int interval, QObject *parent) :
  QObject(parent),
  m_timer(this)
{
  m_timer.setInterval(interval);
  connect(&m_timer, &QTimer::timeout, this, &MSTimer::onTimeout);
}

void MSTimer::addObserver(IMSTimerObserver *ptr)
{
  if (ptr != nullptr && !hasObserver(ptr))
  {
    m_observers.append(ptr);
  }
}

void MSTimer::removeObserver(IMSTimerObserver *ptr)
{
  m_observers.removeAll(ptr);
}

void MSTimer::removeAllObservers()
{
  m_observers.clear();
}

void MSTimer::onTimeout()
{
  for(IMSTimerObserver *ptr: m_observers)
  {
    if (ptr != nullptr)
    {
      ptr->onMsTimer(timerId(), msTime());
    }
  }
}
