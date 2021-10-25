#ifndef XMUTEX_H
#define XMUTEX_H
#include <QMutex>
#include "toolsmacro.h"
#define XMUTEX_LOCK(x) x.lock()
#define XMUTEX_UNLOCK(x) x.unlock()
#define XMUTEX_TRYLOCK(x, y) x.tryLock(y)

#define XMUTEXPTR_LOCK(x) x->lock()
#define XMUTEXPTR_UNLOCK(x) x->unlock()
#define XMUTEXPTR_TRYLOCK(x, y) x->tryLock(y)
#ifndef MUTEX_TRACE
namespace Tools
{
  class XNonRecursiveMutex: public QMutex
  {
  public:
#ifndef USE_SEPARATE_MUTEX
    XNonRecursiveMutex():QMutex(QMutex::NonRecursive){}
#else
    XNonRecursiveMutex():QMutex(){}
#endif
    ~XNonRecursiveMutex() = default;
  }; // XNonRecursiveMutex
#ifndef USE_SEPARATE_MUTEX
  class XRecursiveMutex: public QMutex
#else
    class XRecursiveMutex: public QRecursiveMutex
#endif
  {
  public:
#ifndef USE_SEPARATE_MUTEX
    XRecursiveMutex():QMutex(QMutex::Recursive){}
#else
    XRecursiveMutex():QRecursiveMutex(){}
#endif
    ~XRecursiveMutex() = default;
  }; // XRecursiveMutex
}

#else // MUTEX_TRACE
namespace Tools
{
  class XNonRecursiveMutex
  {
  public:
    explicit XNonRecursiveMutex();
    ~XNonRecursiveMutex();
    void lock(char const* file, char const* function, int line);
    void unlock(char const* file, char const* function, int line);
    bool tryLock(char const* file, char const* function, int line, int timeout);
  private:
    Q_DISABLE_COPY_MOVE(XNonRecursiveMutex)
    TickCounter ticker;
    QMutex m_mutex;
  }; // XNonRecursiveMutex


  class XRecursiveMutex
  {
  public:
    explicit XRecursiveMutex();
    ~XRecursiveMutex();
    void lock(char const* file, char const* function, int line);
    void unlock(char const* file, char const* function, int line);
    bool tryLock(char const* file, char const* function, int line, int timeout);
  private:
    Q_DISABLE_COPY_MOVE(XRecursiveMutex)
    TickCounter ticker;


#ifndef USE_SEPARATE_MUTEX
    QMutex m_mutex;
#else
    QRecursiveMutex m_mutex;
#endif
  }; // XRecursiveMutex

} //namespace Tools
#define XMUTEX_LOCK(x) x.lock(__FILE__, Q_FUNC_INFO, __LINE__)
#define XMUTEX_UNLOCK(x) x.unlock(__FILE__, Q_FUNC_INFO, __LINE__)
#define XMUTEX_TRYLOCK(x, y) x.tryLock(__FILE__, Q_FUNC_INFO, __LINE__, y)


#endif // MUTEX_TRACE else

#endif // XMUTEX_H
