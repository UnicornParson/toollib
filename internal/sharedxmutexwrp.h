#ifndef SHAREDXMUTEXWRP_H
#define SHAREDXMUTEXWRP_H
#include "tools.h"
#include "xmutex.h"
#include <climits>
#ifdef CPP17SUPPORTED
#include <shared_mutex>
#endif
namespace Tools
{
#ifdef CPP17SUPPORTED
  class SharedXMutexWrp
  {
  public:
    explicit SharedXMutexWrp();
    ~SharedXMutexWrp();
    void lockRead(const char* file, const char* function, int line);
    void unlockRead(const char* file, const char* function, int line);
    void lockWrite(const char* file, const char* function, int line);
    void unlockWrite(const char* file, const char* function, int line);
    inline int maxReaders() const {return INT_MAX;}
  private:
    Q_DISABLE_COPY_MOVE(SharedXMutexWrp)
    inline bool isRecursive() const {return true; /* fixed for qt 6 */}
    std::shared_mutex m_mutex;
    TickCounter m_inlockTicker;
    std::atomic<std::thread::id> m_owner;
    qint64 m_lockCount;
  }; // class SharedXMutexWrp
#endif //CPP14PLUS
}
#endif // SHAREDXMUTEXWRP_H
