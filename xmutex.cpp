#include "xmutex.h"
using namespace Tools;

#ifdef MUTEX_TRACE

XNonRecursiveMutex::XNonRecursiveMutex():
  #ifndef USE_SEPARATE_MUTEX
      m_mutex(QMutex::NonRecursive)
  #endif
{
  LOG_TRACE("new XNonRecursiveMutex %lld", intptr_t(this));
}

XNonRecursiveMutex::~XNonRecursiveMutex()
{
  LOG_TRACE("leave XNonRecursiveMutex %lld", intptr_t(this));
}

void XNonRecursiveMutex::lock(char const* file, char const* function, int line)
{
  LOG_TRACE("lock mutex %lld(%s) from %s::%s.%d in thread %p",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            QThread::currentThreadId());

  TickCounter lockTime;
  lockTime.reset();
  m_mutex.lock();
  ticker.reset();
  LOG_TRACE("lock mutex %lld(%s) from %s::%s.%d has been locked in %llu ms, in thread %p",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            lockTime.tickToMs(),
            QThread::currentThreadId());

}

void XNonRecursiveMutex::unlock(char const* file, char const* function, int line)
{
  m_mutex.unlock();
  LOG_TRACE("unlock mutex %lld(%s) from %s::%s.%d in thread %p time in lock state %llu",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            QThread::currentThreadId(),
            ticker.tickToMs());

  ticker.reset();

}

bool XNonRecursiveMutex::tryLock(char const* file, char const* function, int line, int timeout)
{
  LOG_TRACE("tryLock mutex %lld(%s) from %s::%s.%d in thread %p timeout %d",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            QThread::currentThreadId(),
            timeout);

  TickCounter lockTime;
  lockTime.reset();
  bool rc = m_mutex.tryLock(timeout);
  ticker.reset();
  LOG_TRACE("trylock mutex %lld(%s) from %s::%s.%d result: %d, in thread %p",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            rc,
            QThread::currentThreadId());
  return rc;
}
//========================================================================================================

XRecursiveMutex::XRecursiveMutex():
  #ifndef USE_SEPARATE_MUTEX
      m_mutex(QMutex::Recursive)
  #endif
{
  LOG_TRACE("new XRecursiveMutex %lld", intptr_t(this));
}

XRecursiveMutex::~XRecursiveMutex()
{
  LOG_TRACE("leave XRecursiveMutex %lld", intptr_t(this));
}

void XRecursiveMutex::lock(char const* file, char const* function, int line)
{
  LOG_TRACE("lock mutex %lld(%s) from %s::%s.%d in thread %p",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            QThread::currentThreadId());

  TickCounter lockTime;
  lockTime.reset();
  m_mutex.lock();
  ticker.reset();
  LOG_TRACE("lock mutex %lld(%s) from %s::%s.%d has been locked in %llu ms, in thread %p",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            lockTime.tickToMs(),
            QThread::currentThreadId());

}

void XRecursiveMutex::unlock(char const* file, char const* function, int line)
{
  m_mutex.unlock();
  LOG_TRACE("unlock mutex %lld(%s) from %s::%s.%d in thread %p time in lock state %llu",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            QThread::currentThreadId(),
            ticker.tickToMs());

  ticker.reset();

}

bool XRecursiveMutex::tryLock(char const* file, char const* function, int line, int timeout)
{
  LOG_TRACE("tryLock mutex %lld(%s) from %s::%s.%d in thread %p timeout %d",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            QThread::currentThreadId(),
            timeout);

  TickCounter lockTime;
  lockTime.reset();
  bool rc = m_mutex.tryLock(timeout);
  ticker.reset();
  LOG_TRACE("trylock mutex %lld(%s) from %s::%s.%d result: %d, in thread %p",
            intptr_t(this),
            ((m_mode == XMutexModeNonRecursive) ? "NonRecursive" : "Recursive"),
            file,
            function,
            line,
            rc,
            QThread::currentThreadId());
  return rc;
}
#endif
