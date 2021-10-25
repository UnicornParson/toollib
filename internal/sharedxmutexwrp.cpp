#include "sharedxmutexwrp.h"
#ifdef CPP17SUPPORTED

using namespace std;
using namespace Tools;

SharedXMutexWrp::SharedXMutexWrp():
  m_lockCount(0)
{
#ifdef MUTEX_TRACE
  LOG_TRACE("new SharedMutex %lld", intptr_t(this));
#endif
}

SharedXMutexWrp::~SharedXMutexWrp()
{
#ifdef MUTEX_TRACE
  LOG_TRACE("leave SharedMutex %lld", intptr_t(this));
#endif
}

void SharedXMutexWrp::lockRead(const char* file, const char* function, int line)
{

#ifdef MUTEX_TRACE
  LOG_TRACE("lockRead %lld from %s::%s.%d in thread %p",
            intptr_t(this),
            file,
            function,
            line,
            QThread::currentThreadId());

  TickCounter lockTime;
  lockTime.reset();
#else
  UNUSED(file);
  UNUSED(function);
  UNUSED(line);
#endif
  m_mutex.lock_shared();
#ifdef MUTEX_TRACE
  m_inlockTicker.reset();
  LOG_TRACE("lockRead %lld from %s::%s.%d has been locked in %llu ms, in thread %p",
            intptr_t(this),
            file,
            function,
            line,
            lockTime.tickToMs(),
            QThread::currentThreadId());
#endif
}

void SharedXMutexWrp::unlockRead(const char* file, const char* function, int line)
{
  m_mutex.unlock_shared();
#ifdef MUTEX_TRACE
  LOG_TRACE("unlockRead %lld from %s::%s.%d in thread %p. time in lock %llu ms",
            intptr_t(this),
            file,
            function,
            line,
            QThread::currentThreadId(),
            m_inlockTicker.tickToMs());
#else
  UNUSED(file);
  UNUSED(function);
  UNUSED(line);
#endif
}

void SharedXMutexWrp::lockWrite(const char* file, const char* function, int line)
{
#ifdef MUTEX_TRACE
  LOG_TRACE("lockWrite %lld from %s::%s.%d in thread %p",
            intptr_t(this),
            file,
            function,
            line,
            QThread::currentThreadId());

  TickCounter lockTime;
  lockTime.reset();
#else
  UNUSED(file);
  UNUSED(function);
  UNUSED(line);
#endif
  std::thread::id this_id = std::this_thread::get_id();
  if (m_owner == this_id && isRecursive())
  {
    // recursive locking
    m_lockCount++;

#ifdef MUTEX_TRACE
    LOG_TRACE("lockWrite %lld from %s::%s.%d in thread %p. recursive lock. depth: %d",
              intptr_t(this),
              file,
              function,
              line,
              QThread::currentThreadId(),
              m_lockCount);
#endif
  }
  else
  {
    // normal locking
    m_mutex.lock();
    m_owner = this_id;
    m_lockCount = 1;
  }
#ifdef MUTEX_TRACE
  m_inlockTicker.reset();
  LOG_TRACE("lockWrite %lld from %s::%s.%d has been locked in %llu ms, in thread %p",
            intptr_t(this),
            file,
            function,
            line,
            lockTime.tickToMs(),
            QThread::currentThreadId());
#endif
}

void SharedXMutexWrp::unlockWrite(const char* file, const char* function, int line)
{
  if (m_lockCount > 1 && isRecursive())
  {
    // recursive unlocking
    m_lockCount--;
#ifdef MUTEX_TRACE
    LOG_TRACE("lockWrite %lld from %s::%s.%d in thread %p. recursive unlocking. depth: %d",
              intptr_t(this),
              file,
              function,
              line,
              QThread::currentThreadId(),
              m_lockCount);
#endif
  }
  else
  {
    // normal unlocking
    m_owner = std::thread::id();
    m_lockCount = 0;
    m_mutex.unlock();
  }

#ifdef MUTEX_TRACE
  LOG_TRACE("unlockRead %lld from %s::%s.%d in thread %p. time in lock %llu ms",
            intptr_t(this),
            file,
            function,
            line,
            QThread::currentThreadId(),
            m_inlockTicker.tickToMs());
#else
  UNUSED(file);
  UNUSED(function);
  UNUSED(line);
#endif
}

#endif //CPP14PLUS
