#include "functionscheduler.h"
#include <QThread>
#include <QDebug>
#include "tools.h"
using namespace Tools;
static constexpr int DelayStep(100);

FunctionScheduler::FunctionScheduler():
  m_destroyFlag(false),
  m_verbose(false),
  m_refs(0)
{

}

FunctionScheduler::~FunctionScheduler()
{
  if (m_verbose)
  {
    qDebug() << "refs: " << refs();
  }
  m_destroyFlag = true;
  if (!isEmpty())
  {
    qWarning() << "has active routines " << refs();
  }
}

void FunctionScheduler::safeExec(const Routine_t& function,
                                 const ExceptonCallback_t& ec,
                                 const RuntimeErrorCallback_t& rec)
{
  if (self().m_verbose)
  {
    qDebug() << "try to exec routine";
  }
  try
  {
    function();
  }
  catch(const std::runtime_error& re)
  {
    // speciffic handling for runtime_error
    qWarning() << "Runtime error: " << re.what();
    rec(re);

  }
  catch(const std::exception& ex)
  {
    // speciffic handling for all exceptions extending std::exception, except
    // std::runtime_error which is handled explicitly
    qWarning() << "Error occurred: " << ex.what();
    ec(ex);
  }
  catch(...)
  {
    // catch any other errors (that we have no information about)
    qWarning() << "Unknown failure occurred. Possible memory corruption";
    ec(std::exception());
  }
}

void FunctionScheduler::scheduleOnce(const Routine_t& function,
                                     int ms,
                                     const ExceptonCallback_t& ec,
                                     const RuntimeErrorCallback_t& rec)
{
  std::thread t([function, ms, ec, rec]()
  {
    self().m_refs++;
    delayImpl(function, ms, ec, rec);
    self().m_refs--;
    if (self().m_verbose)
    {
      qDebug() << "thread exit. refs:" << refs();
    }
  });
  if (self().m_verbose)
  {
    qDebug() << "new thread. refs:" << refs() << "tid:" << uintptr_t(t.native_handle());
  }
  t.detach();
}

void FunctionScheduler::delayImpl(const Routine_t& function,
                                  int ms,
                                  const ExceptonCallback_t& ec,
                                  const RuntimeErrorCallback_t& rec)
{
  for (int i = 0; i < ms / DelayStep; i++)
  {
    if (self().m_destroyFlag)
    {
      return;
    }
    Tools::mssleep(DelayStep);
  }
  Tools::mssleep(ms % DelayStep);
  if (self().m_destroyFlag)
  {
    return;
  }
  safeExec(function, ec, rec);
}

void FunctionScheduler::setInterval(const Routine_t& function,
                                    int ms,
                                    const ExceptonCallback_t& ec,
                                    const RuntimeErrorCallback_t& rec)
{
  std::thread t([function, ms, ec, rec]()
  {
    self().m_refs++;
    while(!self().m_destroyFlag)
    {
      if (self().m_verbose)
      {
        qDebug() << "new scheduled call. refs:" << refs();
      }
      delayImpl(function, ms, ec, rec);
    }
    self().m_refs--;
    if (self().m_verbose)
    {
      qDebug() << "thread exit. refs:" << refs();
    }
  });
  if (self().m_verbose)
  {
    qDebug() << "new thread. refs:" << refs() << "tid:" << uintptr_t(t.native_handle());
  }
  t.detach();
}

FunctionScheduler& FunctionScheduler::self()
{
  static FunctionScheduler instance;
  return instance;
}

void FunctionScheduler::destroy()
{
  FunctionScheduler& s = self();
  if (self().m_verbose)
  {
    qDebug() << "destroying is started. refs:" << refs();
  }
  s.m_destroyFlag = true;
  quint64 attempt = 0;
  while(!isEmpty())
  {
    qDebug() << "wait for closing activites. count:" << refs() << "attempt:" << attempt;
    Tools::mssleep(DelayStep);
    attempt++;
  }
}

void FunctionScheduler::reset()
{
  FunctionScheduler& s = self();
  if (s.m_verbose)
  {
    qDebug() << "reset";
  }
  destroy();
  s.m_destroyFlag = false;
}
