#ifndef SHAREDXMUTEXNATIVE_H
#define SHAREDXMUTEXNATIVE_H

#include "tools.h"
#include "xmutex.h"

namespace Tools
{
  class SharedXMutexNative
  {
  public:
    SharedXMutexNative();
    ~SharedXMutexNative();
    void lockRead(const char* file, const char* function, int line);
    void unlockRead(const char* file, const char* function, int line);
    void lockWrite(const char* file, const char* function, int line);
    void unlockWrite(const char* file, const char* function, int line);
    inline int maxReaders() const;
  private:
    Q_DISABLE_COPY_MOVE(SharedXMutexNative)
    QSemaphore semaphore;
    XRecursiveMutex mutex;
    TickCounter ticker;
    int m_maxReaders;
  }; // class SharedXMutexNative
}
#endif // SHAREDXMUTEXNATIVE_H
