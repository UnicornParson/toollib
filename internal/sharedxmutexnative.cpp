#include "sharedxmutexnative.h"
#include <climits>
using namespace Tools;
MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR int MAX_READERS(USHRT_MAX);

SharedXMutexNative::SharedXMutexNative()
  : semaphore(MAX_READERS),
    m_maxReaders(MAX_READERS)
{
#ifdef MUTEX_TRACE
  LOG_TRACE("new SharedMutex %lld", intptr_t(this));
#endif
}

SharedXMutexNative::~SharedXMutexNative()
{
#ifdef MUTEX_TRACE
  LOG_TRACE("leave SharedMutex %lld", intptr_t(this));
#endif
}

void SharedXMutexNative::lockRead(const char* file, const char* function, int line)
{
#ifdef MUTEX_TRACE
  LOG_TRACE("lockRead %lld from %s::%s.%d in thread %p readers:%d/%d",
            intptr_t(this),
            file,
            function,
            line,
            QThread::currentThreadId(),
            semaphore.available(),
            MAX_READERS);

  TickCounter lockTime;
  lockTime.reset();
#else
  UNUSED(file);
  UNUSED(function);
  UNUSED(line);
#endif
  semaphore.acquire(1);
#ifdef MUTEX_TRACE
  LOG_TRACE("lockRead %lld from %s::%s.%d has been locked in %llu ms, in thread %p readers:%d/%d",
            intptr_t(this),
            file,
            function,
            line,
            lockTime.tickToMs(),
            QThread::currentThreadId(),
            semaphore.available(),
            MAX_READERS);
#endif
}

void SharedXMutexNative::unlockRead(const char* file, const char* function, int line)
{
  semaphore.release(1);
#ifdef MUTEX_TRACE
  LOG_TRACE("unlockRead %lld from %s::%s.%d in thread %p readers:%d/%d",
            intptr_t(this),
            file,
            function,
            line,
            QThread::currentThreadId(),
            semaphore.available(),
            MAX_READERS);
#else
  UNUSED(file);
  UNUSED(function);
  UNUSED(line);
#endif
}

void SharedXMutexNative::lockWrite(const char* file, const char* function, int line)
{
#ifdef MUTEX_TRACE
  LOG_TRACE("lockWrite %lld from %s::%s.%d in thread %p readers:%d/%d",
            intptr_t(this),
            file,
            function,
            line,
            QThread::currentThreadId(),
            semaphore.available(),
            MAX_READERS);

  TickCounter lockTime;
  lockTime.reset();
#else
  UNUSED(file);
  UNUSED(function);
  UNUSED(line);
#endif

  XMUTEX_LOCK(mutex);
  for (int i = 0; i < maxReaders(); ++i)
  {
    semaphore.acquire(1);
  }
  XMUTEX_UNLOCK(mutex);

#ifdef MUTEX_TRACE
  ticker.reset();
  LOG_TRACE("lockWrite %lld from %s::%s.%d has been locked in %llu ms, in thread %p readers:%d/%d",
            intptr_t(this),
            file,
            function,
            line,
            lockTime.tickToMs(),
            QThread::currentThreadId(),
            semaphore.available(),
            MAX_READERS);
#endif
}

void SharedXMutexNative::unlockWrite(const char* file, const char* function, int line)
{
  semaphore.release(m_maxReaders);
#ifdef MUTEX_TRACE
  LOG_TRACE("unlockWrite %lld from %s::%s.%d in thread %p time in lock state %llu readers:%d/%d",
            intptr_t(this),
            file,
            function,
            line,
            QThread::currentThreadId(),
            ticker.tickToMs(),
            semaphore.available(),
            MAX_READERS);
#else
  UNUSED(file)
  UNUSED(function)
  UNUSED(line)
#endif
}

int SharedXMutexNative::maxReaders() const
{
  return MAX_READERS;
}


